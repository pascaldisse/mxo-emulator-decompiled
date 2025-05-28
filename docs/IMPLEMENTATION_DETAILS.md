# MxoEmu Implementation Details

This document provides detailed information about the implementation of key features and systems in the MxoEmu Matrix Online server emulator, as reconstructed through reverse engineering efforts.

## Server Architecture

The emulator follows a multi-component architecture divided into three primary server components:

### 1. Authentication Server

The Authentication Server manages user accounts, authentication, and character selection:

- **Security**: Implements RSA encryption for secure password transmission
- **Account Management**: Handles account creation, password changes, and session management
- **Character Selection**: Manages character creation, selection, and deletion
- **Protocol**: Uses TCP for reliable communication

```
Authentication Flow:
1. Client connects to Authentication Server
2. Server sends challenge and public key
3. Client responds with encrypted credentials
4. Server validates credentials against database
5. If valid, server generates a session key
6. Server sends session key to client
7. Client selects a character
8. Server transfers client to Game Server
```

### 2. Game Server

The Game Server manages the virtual world, player characters, and object interactions:

- **World State**: Maintains game world state, objects, and district information
- **Player Management**: Tracks player positions, state, and interactions
- **Object System**: Handles creation, updates, and destruction of game objects
- **Spatial Queries**: Provides spatial query functionality through NavMesh
- **Protocol**: Uses UDP with a reliability layer for efficient communication

```
Game Object Hierarchy:
GameObject (base class)
├── PlayerObject (player characters)
├── NPCObject (non-player characters)
├── ItemObject (items and equipment)
├── InteractiveObject (doors, computers, etc.)
└── EnvironmentObject (static world objects)
```

### 3. Margin Server

The Margin Server (named after "The Margin" character in MxO) handles mission content and narrative elements:

- **Mission System**: Manages mission availability, progression, and completion
- **Dialogue System**: Handles NPC dialogue trees and player choices
- **Storyline Events**: Coordinates scheduled storyline events and updates
- **Protocol**: Uses TCP for reliable communication

## Networking

### Protocol Structure

The network protocol uses a structured binary format with message types and blocks:

1. **Common Header** (8 bytes):
   - Magic value (1 byte)
   - Protocol version (1 byte)
   - Message type (2 bytes)
   - Message length (4 bytes)

2. **Game Protocol Extensions** (UDP packets):
   - Sequence number (2 bytes)
   - Acknowledgment number (2 bytes)
   - Flags (1 byte)
   - Block count (1 byte)

3. **Message Blocks**:
   - Block type (2 bytes)
   - Block length (2 bytes)
   - Block data (variable)

### Reliability Layer

UDP packets implement a reliability system:

- **Sequence Numbers**: Track packet order
- **Acknowledgments**: Confirm packet receipt
- **Retransmission**: Resend unacknowledged packets
- **Fragmentation**: Split and reassemble large messages
- **Encryption**: Optional encryption for sensitive data

## Memory Management

### Buffer System

The `ByteBuffer` class provides efficient binary data handling:

- Dynamic resizing for growing buffers
- Position tracking for read/write operations
- Type-safe reading and writing of primitive types
- String operations with null termination
- Serialization/deserialization interface

## Spatial System

### LocationVector

The `LocationVector` class represents positions and orientations in 3D space:

- 3D coordinates (x, y, z)
- Orientation (o) in radians
- Distance calculations (2D and 3D)
- Vector operations (addition, subtraction)
- Movement methods (forward, angular)

### NavMesh System

The `NavMeshManager` provides pathfinding and spatial queries:

- **Pathfinding**: A* algorithm on navigation mesh
- **Line of Sight**: Visibility checks between points
- **Position Validation**: Check if positions are valid (on navmesh)
- **Random Points**: Generate random valid positions
- **Distance Queries**: Calculate path distances along navmesh

## Game Object System

The `GameObject` class forms the base of all interactive objects:

- **Properties**: Position, orientation, district, name, state flags
- **Serialization**: Methods for binary serialization/deserialization
- **Messaging**: Create, update, and destroy message generation
- **Interaction**: Handle interactions from other objects
- **Custom Properties**: Support for arbitrary key-value properties

## Database Schema

The database uses a relational schema with the following key tables:

### accounts
- `account_id`: Primary key
- `username`: Login name
- `password_hash`: SHA-1 hash of password+salt
- `password_salt`: Random salt for hashing
- Authentication metadata (last login, IP, etc.)

### characters
- `character_id`: Primary key
- `account_id`: Foreign key to accounts
- `world_id`: Foreign key to worlds
- `character_handle`: Unique character name
- Character attributes (name, level, stats, etc.)
- Position data (x, y, z, rotation, district)

### character_appearance
- `character_id`: Primary key/foreign key
- `rsi_data`: Binary appearance data
- Animation and mood state

### Additional tables
- `worlds`: Game world definitions
- `character_inventory`: Character items
- `character_skills`: Character abilities

## Thread Management

The server uses a multi-threaded architecture:

- **Main Thread**: Master controller and signal handling
- **Authentication Thread**: Handles auth server operations
- **Game Thread**: Manages game world and updates
- **Margin Thread**: Processes mission and narrative content
- **Console Thread**: Processes admin commands

Thread safety is maintained through mutex locks and thread-safe data structures.

## Configuration System

The server uses a configuration file (Reality.conf) with sections for:

- Database connection settings
- Network ports and addresses
- World parameters
- Security settings
- Performance tuning
- Logging configuration

## Singleton Pattern

Core managers implement the Singleton pattern to ensure only one instance exists:

- `Master`: Central controller
- `AuthServer`: Authentication server
- `GameServer`: Game world server
- `MarginServer`: Mission and narrative server
- `WorldManager`: World state manager

## Mission System

Missions follow a structured format:

- **Mission Definition**: Basic mission data (name, description, etc.)
- **Prerequisites**: Required level, faction, prior missions
- **Objectives**: Multiple objectives with progress tracking
- **Rewards**: Experience, information, items

## Dialogue System

NPC dialogue is implemented as dialogue trees:

- **Dialogue Node**: Text, speaker, and animation
- **Dialogue Options**: Player response choices
- **Conditions**: Requirements for options to appear
- **Actions**: Effects when options are selected

## Event System

The PlayerObject implements an event system for delayed actions:

- Events are scheduled with a callback and delay time
- Event types include JACKOUT, TELEPORT, ABILITY_END, etc.
- Events are processed during the update cycle

## Notes on Implementation

1. **Missing Components**: Some referenced classes like MissionManager, DialogueManager, MarginHandler, and socket classes are not yet implemented.

2. **Placeholder Code**: Some cryptographic functions contain placeholder implementations that would need to be replaced with actual cryptographic operations.

3. **External Dependencies**: The codebase appears to rely on external libraries like Crypto++ for cryptography and Detour for navigation meshes.

4. **Client Compatibility**: The protocol implementation aims to be compatible with the original Matrix Online client, though some details may require further refinement based on client behavior.

## Future Work

The following areas need additional development:

1. **Combat System**: Implementation of the combat mechanics, abilities, and effects
2. **AI System**: NPC behavior and pathfinding implementation
3. **Mission Content**: Actual mission definitions and objectives
4. **Dialogue Content**: NPC dialogue scripts and responses
5. **Item System**: Complete item definitions and functionality
6. **Protocol Compatibility**: Further refinement of packet structures
7. **Security Hardening**: Full implementation of cryptographic functions

## Technical Challenges

Several challenges were encountered during reconstruction:

1. **Message Formats**: Determining exact packet structures without complete client source
2. **Cryptographic Details**: Reconstructing the exact encryption parameters
3. **Database Schema**: Inferring the complete schema from available code
4. **Protocol Timing**: Understanding packet timing requirements
5. **Client Compatibility**: Ensuring backward compatibility with the original client