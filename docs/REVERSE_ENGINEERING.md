# Matrix Online Emulator Reverse Engineering Documentation

This document provides a detailed technical analysis of the MxoEmu (Reality) Matrix Online emulator codebase, based on reverse engineering efforts. It explains the architecture, key components, and implementation details.

## Overall Architecture

The MxoEmu emulator follows a multi-threaded client-server architecture that consists of several core components:

1. **Master Controller**: The central coordinator that initializes and manages all server components
2. **Authentication Server**: Handles user authentication and character management
3. **Game Server**: Manages the game world, objects, and player interactions
4. **Margin Server**: Handles mission system and narrative content
5. **Database Layer**: Provides persistent storage for accounts, characters, and world data

The server uses a thread pool to manage these components concurrently, with separate threads for authentication, game world, margin, and console interfaces.

## Thread Management

The server employs a thread pool design pattern:

```
Master Controller
├── ThreadPool
    ├── AuthRunnable Thread
    ├── MarginRunnable Thread
    ├── GameRunnable Thread
    └── ConsoleThread
```

Each component runs in its own thread, with the Master controller monitoring for shutdown signals. This allows the server to handle multiple clients and server functions concurrently.

## Key Classes

### Master Class

```cpp
class Master : public Singleton<Master> {
    // Core server management
}
```

The Master class is implemented as a Singleton and serves as the central controller for the entire emulator. It:
- Initializes the database connection
- Sets up signal handling for graceful shutdown
- Creates and manages server threads
- Coordinates between components

The main server loop is implemented in `Master::Run()`, which initializes all subsystems and enters a wait loop that monitors for shutdown signals.

### AuthServer Class

```cpp
class AuthServer : public Singleton<AuthServer> {
    // Authentication management
}
```

The AuthServer handles all authentication-related functions:
- User login and session management
- Account creation and password management
- Character creation and selection
- Secure communication using RSA encryption

It implements cryptographic functions for:
- RSA encryption/decryption for secure communication
- SHA-1 password hashing with random salts
- Digital signatures for verifying server authenticity

### ByteBuffer Class

```cpp
class ByteBuffer {
    // Binary data handling
}
```

This utility class provides functions for:
- Reading and writing binary data with proper endianness
- Buffer management (resizing, clearing)
- Position tracking for read/write operations
- Template-based type handling
- String operations

ByteBuffer is used extensively throughout the codebase for network packet serialization and message passing.

### LocationVector Class

```cpp
class LocationVector {
    // 3D position and orientation
}
```

This class represents positions and orientations in the game world:
- 3D coordinates (x, y, z)
- Orientation angle (o)
- Distance calculations (2D and 3D)
- Vector operations for movement

LocationVector is used for player positions, object placement, and movement calculations.

### PlayerObject Class

```cpp
class PlayerObject {
    // Player character state and behavior
}
```

This class represents a player character in the game world:
- Character stats (health, inner strength, experience, etc.)
- Position and movement
- Appearance data
- RPC handlers for client-server interaction
- Event system for timed actions

PlayerObject has numerous RPC handlers to process client requests like chat, movement, emotes, and object interactions.

## Database Schema

The database schema includes the following key tables:

### accounts
- `account_id`: Primary key
- `username`: User login name
- `password_hash`: SHA-1 hashed password
- `password_salt`: Random salt for password hashing
- `last_login`: Timestamp of last login

### worlds
- `world_id`: Primary key
- `world_name`: Name of the game world
- `status`: Server status (online, offline, etc.)

### characters
- `character_id`: Primary key
- `account_id`: Foreign key to accounts table
- `world_id`: Foreign key to worlds table
- `character_handle`: Unique character name
- `first_name`, `last_name`: Character's full name
- `experience`, `information`: Character progression metrics
- `health_current`, `health_max`: Health values
- `innerstr_current`, `innerstr_max`: Inner strength values
- `profession`, `level`, `alignment`: Character attributes
- `pos_x`, `pos_y`, `pos_z`, `rotation`: Position and orientation
- `district`: Current district ID
- `is_online`: Online status flag

## Network Protocol

The network protocol appears to be implemented with these characteristics:

1. **Authentication Server**: Uses TCP for reliable connection-based communication
2. **Game Server**: Likely uses UDP for frequent position updates with a reliability layer
3. **Custom Packet Format**: Messages are organized into blocks with headers and data
4. **Encryption**: RSA encryption for sensitive data
5. **Authentication**: Challenge-response authentication with hashed passwords

## Security Implementation

The emulator implements several security measures:

1. **Password Security**:
   - Passwords are stored as SHA-1 hashes
   - Each password has a unique random salt
   - Original passwords are never stored

2. **Communication Security**:
   - RSA encryption for sensitive data
   - Digital signatures for message authenticity
   - Public/private key pairs for secure key exchange

3. **Session Management**:
   - Session-based authentication
   - Timeout handling for inactive sessions

## Event System

The PlayerObject class implements an event system for timed actions:

```cpp
void addEvent(eventType type, eventFunc func, float activationTime);
size_t cancelEvents(eventType type);
```

This system allows for scheduling future actions like:
- Character "jackout" (disconnect)
- Timed ability effects
- Delayed teleportation

Events are stored in a list and processed during the update cycle.

## Message Handling

The emulator uses a Remote Procedure Call (RPC) approach for client-server communication:

1. Client sends a command message
2. Server routes it to the appropriate handler based on message type
3. Handler processes the command and generates a response

The PlayerObject class contains numerous RPC handlers for different message types:

```cpp
// RPC handler type
typedef void (PlayerObject::*RPCHandler)(ByteBuffer &srcCmd);

// Handler maps
std::map<uint8_t, RPCHandler> m_RPCbyte;
std::map<uint16_t, RPCHandler> m_RPCshort;
```

Messages are categorized by byte or short identifiers and mapped to specific handler functions.

## Key Algorithms

### Authentication Flow

1. Client connects to authentication server
2. Server sends public key and challenge
3. Client encrypts login credentials with server's public key
4. Server verifies credentials against database
5. On success, server generates a session key
6. Session key is used for subsequent communication

### Character Creation

1. Verify account exists
2. Verify world exists
3. Check if character handle is unique
4. Create character record with default values
5. Initialize character appearance data

### Password Hashing

1. Generate a random salt
2. Concatenate salt and password
3. Compute SHA-1 hash of the combined string
4. Store salt and hash in database

## Configuration System

The emulator uses a configuration file (Reality.conf) for server settings:

- Database connection parameters
- Network listen ports
- Threading configuration
- Logging settings

The configuration is loaded at startup by the Master class.

## Future Development Considerations

Based on the codebase analysis, several areas appear incomplete:

1. **Game Mechanics**: Combat, skills, and abilities
2. **Mission System**: The Margin server implementation
3. **World Simulation**: Physics, collision, and environment interaction
4. **NPC AI**: Non-player character behavior and interactions
5. **Network Protocol**: Full implementation of all packet types

## Conclusion

The MxoEmu emulator provides a foundation for recreating The Matrix Online server, with implementations for authentication, character management, and basic world interaction. While some components are well-developed, others appear to be placeholders or incomplete implementations.

The codebase demonstrates good software engineering practices including:
- Design patterns (Singleton, Thread Pool)
- Separation of concerns
- Consistent error handling
- Security considerations
- Documentation

Further development would require completing the game-specific systems and implementing the full network protocol to match the original client's expectations.