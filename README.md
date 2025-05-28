# MxoEmu Reconstructed

This repository contains a reconstructed and reorganized version of the MxoEmu (Reality) Matrix Online emulator, based on the original source code and additional reverse engineering efforts. The code is provided for educational purposes.

## About MxoEmu

MxoEmu (Reality) is a server emulator for The Matrix Online, originally developed by Rajko Stojadinovic and the MxO community. It aims to recreate the functionality of the original game servers that were shut down in 2009.

## Project Structure

The repository is organized into the following main directories:

- `src/` - Main source code
  - `auth/` - Authentication server
  - `game/` - Game world server
  - `margin/` - Margin server (mission system)
  - `common/` - Shared code
  - `database/` - Database access layer
  - `network/` - Network protocol implementation
- `include/` - Header files
- `lib/` - External libraries
- `sql/` - Database schema and queries
- `conf/` - Configuration examples
- `docs/` - Additional documentation

## Documentation

Comprehensive documentation is available in the `docs/` directory:

- **ARCHITECTURE.md** - Overall architecture and component relationships
- **REVERSE_ENGINEERING.md** - Technical analysis based on reverse engineering
- **NETWORK_PROTOCOL.md** - Detailed documentation of the network protocol

## Key Components

### Authentication Server

The Authentication Server handles:
- User login and registration
- Character creation and selection
- Secure communication with RSA encryption
- Session management

### Game Server

The Game Server manages:
- World simulation
- Player movement and state
- Object interactions
- Chat and communication

### Margin Server

The Margin Server (named after the character "The Margin" from the game) handles:
- Mission system
- Narrative content
- Storyline progression

## Building the Emulator

### Requirements

- C++11 compliant compiler
- MySQL/MariaDB 5.5+
- Boost 1.55+
- OpenSSL 1.0.1+
- CMake 3.5+
- Crypto++ 5.6+

### Build Instructions

1. Clone this repository
2. Create a build directory: `mkdir build && cd build`
3. Configure with CMake: `cmake ..`
4. Build: `make`
5. Install (optional): `make install`

## Configuration

1. Copy the example configuration: `cp conf/Reality.conf.example conf/Reality.conf`
2. Edit the configuration file to match your environment
3. Set up the database: `mysql -u root -p < sql/database_schema.sql`
4. Create an account for the database: `GRANT ALL ON mxoemu.* TO 'mxouser'@'localhost' IDENTIFIED BY 'password';`

## Running the Emulator

1. Start the server: `./mxoemu`
2. The server will start the Authentication, Game, and Margin components
3. Connect with a Matrix Online client configured to use your server address

## Database Schema

The database schema includes tables for:
- Accounts and authentication
- Characters and their attributes
- Inventory and items
- Skills and abilities
- Game worlds and districts

The full schema can be found in `sql/database_schema.sql`.

## Network Protocol

The emulator implements The Matrix Online's custom network protocol:
- Authentication server uses TCP
- Game server uses UDP with a reliability layer
- Margin server uses TCP
- Encrypted with RSA for sensitive data

See `docs/NETWORK_PROTOCOL.md` for complete protocol documentation.

## Original License

MxoEmu was originally released under the GNU Affero General Public License v3. This reconstructed version maintains that license for all original code.

## Disclaimer

This project is not affiliated with Warner Bros., Monolith Productions, Sega, or Sony Online Entertainment. It is a community-driven educational project intended for research and historical preservation purposes only.

## Acknowledgments

Special thanks to:
- Rajko Stojadinovic and the original MxoEmu developers
- The Matrix Online community for their preservation efforts
- All contributors to this reconstructed version