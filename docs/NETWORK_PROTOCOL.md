# Matrix Online Network Protocol

This document describes the network protocol used by The Matrix Online client to communicate with the server, based on reverse engineering of the MxoEmu codebase.

## Overview

The Matrix Online uses a custom binary protocol with different transport mechanisms for different server components:

1. **Authentication Server**: TCP-based reliable communication
2. **Game Server**: UDP-based communication with reliability layer
3. **Margin Server**: TCP-based reliable communication

## Packet Structure

### Common Header Structure

All packets begin with a common header:

```
0      1      2      3      4      5      6      7      8
+------+------+------+------+------+------+------+------+
| Magic| Version |  Type   |       Packet Length       |
+------+------+------+------+------+------+------+------+
```

- **Magic (1 byte)**: Constant value 0xA5 identifying a valid packet
- **Version (1 byte)**: Protocol version (typically 0x01)
- **Type (2 bytes)**: Packet type identifier
- **Packet Length (4 bytes)**: Total length of the packet including header

### Authentication Packets

Authentication packets use a challenge-response model with RSA encryption:

#### AUTH_CHALLENGE (Server to Client)

```
0      1      2      3      4      5      6      7      8
+------+------+------+------+------+------+------+------+
| Common Header                                         |
+------+------+------+------+------+------+------+------+
| Challenge Length       | Challenge Data ...           |
+------+------+------+------+------+------+------+------+
| RSA Key Length         | RSA Public Key ...           |
+------+------+------+------+------+------+------+------+
| Signature Length       | Signature ...                |
+------+------+------+------+------+------+------+------+
```

#### AUTH_RESPONSE (Client to Server)

```
0      1      2      3      4      5      6      7      8
+------+------+------+------+------+------+------+------+
| Common Header                                         |
+------+------+------+------+------+------+------+------+
| Response Length        | Encrypted Response ...       |
+------+------+------+------+------+------+------+------+
```

The encrypted response contains:
- Username
- Password
- Challenge response

#### AUTH_RESULT (Server to Client)

```
0      1      2      3      4      5      6      7      8
+------+------+------+------+------+------+------+------+
| Common Header                                         |
+------+------+------+------+------+------+------+------+
| Result Code            | Session Key Length           |
+------+------+------+------+------+------+------+------+
| Encrypted Session Key ...                             |
+------+------+------+------+------+------+------+------+
```

Result codes:
- 0: Success
- 1: Invalid credentials
- 2: Account banned
- 3: Server full
- 4: Already logged in
- 5: Invalid client version

### Game Server Packets

Game server packets use a sequence-acknowledgment system for reliability:

#### Common Game Packet Header

```
0      1      2      3      4      5      6      7      8
+------+------+------+------+------+------+------+------+
| Common Header                                         |
+------+------+------+------+------+------+------+------+
| Sequence Number        | Ack Number                   |
+------+------+------+------+------+------+------+------+
| Flags  | Block Count   | Block Data ...               |
+------+------+------+------+------+------+------+------+
```

- **Sequence Number (2 bytes)**: Incrementing packet sequence
- **Ack Number (2 bytes)**: Acknowledging received packets
- **Flags (1 byte)**:
  - 0x01: Reliable (requires ack)
  - 0x02: Encrypted
  - 0x04: Compressed
  - 0x08: Fragment
- **Block Count (1 byte)**: Number of message blocks in the packet

### Message Block Structure

Each game packet can contain multiple message blocks:

```
0      1      2      3      4      5      6      7      8
+------+------+------+------+------+------+------+------+
| Block Type            | Block Length                  |
+------+------+------+------+------+------+------+------+
| Block Data ...                                        |
+------+------+------+------+------+------+------+------+
```

- **Block Type (2 bytes)**: Type of message block
- **Block Length (2 bytes)**: Length of block data
- **Block Data**: Variable length data specific to the block type

## Message Types

### Authentication Messages

| Type ID | Name | Description |
|---------|------|-------------|
| 0x0001 | AUTH_CHALLENGE | Server sends challenge and public key |
| 0x0002 | AUTH_RESPONSE | Client sends encrypted credentials |
| 0x0003 | AUTH_RESULT | Server sends authentication result |
| 0x0004 | CHAR_LIST_REQUEST | Client requests character list |
| 0x0005 | CHAR_LIST_RESPONSE | Server sends character list |
| 0x0006 | CHAR_CREATE_REQUEST | Client requests character creation |
| 0x0007 | CHAR_CREATE_RESPONSE | Server confirms character creation |
| 0x0008 | CHAR_DELETE_REQUEST | Client requests character deletion |
| 0x0009 | CHAR_DELETE_RESPONSE | Server confirms character deletion |
| 0x000A | WORLD_LIST_REQUEST | Client requests available worlds |
| 0x000B | WORLD_LIST_RESPONSE | Server sends world list |
| 0x000C | CHAR_SELECT_REQUEST | Client selects a character |
| 0x000D | CHAR_SELECT_RESPONSE | Server confirms character selection |

### Game Server Messages

| Type ID | Name | Description |
|---------|------|-------------|
| 0x1001 | GAME_HANDSHAKE | Initial game server handshake |
| 0x1002 | GAME_SESSION | Session establishment |
| 0x1003 | PLAYER_MOVEMENT | Player movement update |
| 0x1004 | PLAYER_STATE | Player state update |
| 0x1005 | WORLD_STATE | World state update |
| 0x1006 | OBJECT_CREATE | Create game object |
| 0x1007 | OBJECT_UPDATE | Update game object |
| 0x1008 | OBJECT_DESTROY | Destroy game object |
| 0x1009 | CHAT_MESSAGE | Chat message |
| 0x100A | PLAYER_COMMAND | Player command |
| 0x100B | REGION_LOAD | Region loading notification |
| 0x100C | JACKOUT_REQUEST | Player jackout (disconnect) request |
| 0x100D | JACKOUT_RESPONSE | Server confirms jackout |

### Margin Server Messages

| Type ID | Name | Description |
|---------|------|-------------|
| 0x2001 | MISSION_LIST_REQUEST | Client requests available missions |
| 0x2002 | MISSION_LIST_RESPONSE | Server sends mission list |
| 0x2003 | MISSION_ACCEPT | Client accepts a mission |
| 0x2004 | MISSION_UPDATE | Server updates mission status |
| 0x2005 | MISSION_COMPLETE | Mission completion notification |
| 0x2006 | DIALOGUE_REQUEST | Client requests NPC dialogue |
| 0x2007 | DIALOGUE_RESPONSE | Server sends dialogue options |
| 0x2008 | DIALOGUE_CHOICE | Client selects dialogue option |

## RPC System

The PlayerObject class implements a Remote Procedure Call system for handling client commands:

```cpp
// RPC handler type
typedef void (PlayerObject::*RPCHandler)(ByteBuffer &srcCmd);

// Handler maps
std::map<uint8_t, RPCHandler> m_RPCbyte;
std::map<uint16_t, RPCHandler> m_RPCshort;
```

Commands are categorized by their ID size (byte or short) and mapped to handler functions. When a PLAYER_COMMAND message is received, the server extracts the command ID and routes it to the appropriate handler.

## Encryption

The protocol uses RSA encryption for sensitive data like authentication:

1. **Key Exchange**:
   - Server generates RSA key pair
   - Public key is sent to client
   - Digital signature verifies key authenticity

2. **Authentication**:
   - Client encrypts credentials with server's public key
   - Server decrypts using private key
   - Session key is generated for subsequent communication

3. **Session Encryption**:
   - Session key is used with symmetric encryption (likely AES)
   - Only packets with the ENCRYPTED flag use encryption

## Reliability Layer

The game server uses a sequence-acknowledgment system for reliability:

1. **Sequence Numbers**:
   - Each packet has a sequence number
   - Receiver acknowledges receipt with ack number

2. **Retransmission**:
   - Sender retransmits unacknowledged packets after timeout
   - Window-based flow control limits outstanding packets

3. **Fragmentation**:
   - Large messages are split into multiple packets
   - FRAGMENT flag indicates a fragmented message
   - Fragments are reassembled by the receiver

## Connection States

Connections progress through these states:

1. **Disconnected**: No connection established
2. **Connecting**: Initial handshake in progress
3. **Authenticating**: Authentication in progress
4. **Character Selection**: Selecting or creating a character
5. **World Selection**: Selecting a game world
6. **Connected**: Fully connected and in game
7. **Disconnecting**: Graceful disconnection in progress

## Typical Message Flow

### Authentication Flow

1. Client connects to authentication server
2. Server sends AUTH_CHALLENGE
3. Client sends AUTH_RESPONSE with encrypted credentials
4. Server sends AUTH_RESULT with session key
5. Client requests character list (CHAR_LIST_REQUEST)
6. Server sends character list (CHAR_LIST_RESPONSE)
7. Client selects character (CHAR_SELECT_REQUEST)
8. Server confirms selection (CHAR_SELECT_RESPONSE)
9. Client disconnects from auth server and connects to game server

### Game Connection Flow

1. Client connects to game server
2. Client sends GAME_HANDSHAKE with session key
3. Server validates session and sends GAME_SESSION
4. Server sends initial WORLD_STATE messages
5. Client sends REGION_LOAD when ready
6. Server spawns player and nearby objects
7. Normal gameplay communication begins

## References

This protocol documentation is based on reverse engineering of the MxoEmu codebase and may not perfectly match the original Matrix Online protocol. It represents a best-effort reconstruction based on available information.