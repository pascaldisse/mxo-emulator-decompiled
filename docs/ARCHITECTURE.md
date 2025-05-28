# MxoEmu Architecture Documentation

## Overview

MxoEmu (Reality) is a server emulator for The Matrix Online, designed to recreate the functionality of the original game servers. This document outlines the major architectural components and their relationships.

## Server Architecture

The emulator follows a multi-threaded architecture with separate components handling different aspects of the game server:

```
Master Controller
├── Authentication Server
│   ├── Account Management
│   └── Character Selection
├── Margin Server
│   ├── Mission System
│   └── Narrative Content
├── Game Server
│   ├── World Simulation
│   ├── Player Management
│   └── Object Interaction
└── Console Interface
    └── Admin Commands
```

## Core Components

### Master Controller

The Master class is the central controller for the emulator. It initializes all subsystems, manages the server lifecycle, and coordinates between components. Key responsibilities include:

- Initializing the database connection
- Starting server threads
- Managing signal handling
- Coordinating shutdown

### Authentication Server

The AuthServer handles user authentication, account creation, and character selection:

- User login and session management
- Account creation and password management
- Character creation and selection
- RSA encryption for secure communication
- Session key generation

### Margin Server

The MarginServer handles mission and narrative content:

- Mission assignment and tracking
- Storyline progression
- NPC dialogue and interactions
- Quest rewards and consequences

### Game Server

The GameServer manages the virtual world and player interactions:

- World state management
- Player movement and position tracking
- Character interactions
- Chat systems
- Environment manipulation

## Threading Model

The emulator uses a multi-threaded architecture:

- Main thread: Master controller and server management
- Authentication thread: User authentication and session management
- Margin thread: Mission and narrative content
- Game thread: World simulation and player interactions
- Console thread: Admin commands and server control

## Database Schema

The database schema includes the following key tables:

- `accounts`: User account information
- `characters`: Character data and statistics
- `worlds`: Game world definitions
- `character_appearance`: Character appearance data

## Network Protocol

The emulator implements The Matrix Online's custom network protocol:

- Authentication server: TCP-based communication
- Margin server: TCP-based communication
- Game server: UDP-based communication with reliability layer
- Custom packet formats and encryption

## Message Processing

Messages between the client and server follow a structured format:

- Messages are organized into blocks
- Each block can contain multiple sub-packets
- Messages are sequenced for reliable delivery
- Some messages are encrypted for security

## Object System

The object system manages game entities:

- Player objects represent player characters
- Dynamic objects represent interactive elements
- Static objects represent environmental elements
- Object IDs are unique within a world
- Objects have properties and methods for interaction

## Future Extensions

This documentation will be expanded as more of the emulator is reconstructed, with particular focus on:

- NavMesh integration
- NPC AI system
- Combat mechanics
- Custom content creation
- Scripting interface