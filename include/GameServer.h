#ifndef _GAME_SERVER_H_
#define _GAME_SERVER_H_

#include "Singleton.h"
#include "GameHandler.h"
#include "GameSocket.h"
#include "ByteBuffer.h"
#include "PlayerObject.h"
#include "WorldManager.h"
#include "MessageTypes.h"

#include <Sockets/ListenSocket.h>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <memory>

/**
 * @brief Game server
 * 
 * The GameServer manages the game world, player interactions, object updates,
 * and world state.
 */
class GameServer : public Singleton<GameServer>
{
public:
    /**
     * @brief Default constructor
     */
    GameServer();
    
    /**
     * @brief Destructor
     */
    ~GameServer();
    
    /**
     * @brief Start the game server
     * 
     * Initializes and starts the server, binding to the configured port.
     */
    void Start();
    
    /**
     * @brief Stop the game server
     * 
     * Stops the server and releases resources.
     */
    void Stop();
    
    /**
     * @brief Process a server loop iteration
     * 
     * Handles pending socket events, updates game state, and processes game logic.
     * 
     * @param diff Time difference since last update in milliseconds
     */
    void Loop(uint32_t diff);
    
    /**
     * @brief Add a player to the server
     * 
     * @param player Player object to add
     * @return true if successful, false otherwise
     */
    bool AddPlayer(std::shared_ptr<PlayerObject> player);
    
    /**
     * @brief Remove a player from the server
     * 
     * @param playerId Player ID to remove
     * @return true if successful, false otherwise
     */
    bool RemovePlayer(uint32_t playerId);
    
    /**
     * @brief Get a player by ID
     * 
     * @param playerId Player ID to find
     * @return Shared pointer to player object, or nullptr if not found
     */
    std::shared_ptr<PlayerObject> GetPlayer(uint32_t playerId);
    
    /**
     * @brief Get a player by character handle
     * 
     * @param handle Character handle to find
     * @return Shared pointer to player object, or nullptr if not found
     */
    std::shared_ptr<PlayerObject> GetPlayerByHandle(const std::string& handle);
    
    /**
     * @brief Get all players in a district
     * 
     * @param districtId District ID to query
     * @return Vector of player objects in the district
     */
    std::vector<std::shared_ptr<PlayerObject>> GetPlayersInDistrict(uint8_t districtId);
    
    /**
     * @brief Get all players in the server
     * 
     * @return Map of player ID to player object
     */
    const std::map<uint32_t, std::shared_ptr<PlayerObject>>& GetAllPlayers() const;
    
    /**
     * @brief Get the world manager
     * 
     * @return Reference to the world manager
     */
    WorldManager& GetWorldManager();
    
    /**
     * @brief Send a message to a specific player
     * 
     * @param playerId Player ID to send to
     * @param message Message to send
     * @return true if successful, false otherwise
     */
    bool SendMessageToPlayer(uint32_t playerId, const MessageBase& message);
    
    /**
     * @brief Send a message to all players in a district
     * 
     * @param districtId District ID to send to
     * @param message Message to send
     * @param exceptPlayerId Player ID to exclude (optional)
     */
    void BroadcastToDistrict(uint8_t districtId, const MessageBase& message, uint32_t exceptPlayerId = 0);
    
    /**
     * @brief Send a message to all players in the server
     * 
     * @param message Message to send
     * @param exceptPlayerId Player ID to exclude (optional)
     */
    void BroadcastToAll(const MessageBase& message, uint32_t exceptPlayerId = 0);
    
    /**
     * @brief Create a game object in the world
     * 
     * @param objectType Type of object to create
     * @param position Position to create at
     * @param data Additional object data
     * @return Created object ID, or 0 on failure
     */
    uint32_t CreateObject(uint16_t objectType, const LocationVector& position, ByteBuffer& data);
    
    /**
     * @brief Update a game object
     * 
     * @param objectId Object ID to update
     * @param data Update data
     * @return true if successful, false otherwise
     */
    bool UpdateObject(uint32_t objectId, ByteBuffer& data);
    
    /**
     * @brief Destroy a game object
     * 
     * @param objectId Object ID to destroy
     * @return true if successful, false otherwise
     */
    bool DestroyObject(uint32_t objectId);
    
    /**
     * @brief Get server statistics
     * 
     * @param totalPlayers Total number of players
     * @param activePlayers Active players in the last 5 minutes
     * @param objectCount Number of active game objects
     * @param uptime Server uptime in seconds
     */
    void GetStats(uint32_t& totalPlayers, uint32_t& activePlayers, uint32_t& objectCount, uint32_t& uptime);
    
    /**
     * @brief Get the next available game object ID
     * 
     * @return Next available object ID
     */
    uint32_t GetNextObjectId();

private:
    /**
     * @brief Update all players
     * 
     * @param diff Time difference since last update in milliseconds
     */
    void UpdatePlayers(uint32_t diff);
    
    /**
     * @brief Update all game objects
     * 
     * @param diff Time difference since last update in milliseconds
     */
    void UpdateObjects(uint32_t diff);
    
    /**
     * @brief Update world state
     * 
     * @param diff Time difference since last update in milliseconds
     */
    void UpdateWorld(uint32_t diff);
    
    /**
     * @brief Process movement updates
     */
    void ProcessMovement();
    
    /**
     * @brief Check for player timeout
     */
    void CheckPlayerTimeout();

    /**
     * @brief Game socket handler
     */
    GameHandler gameSocketHandler;
    
    /**
     * @brief Game listen socket
     */
    typedef ListenSocket<GameSocket> GameListenSocket;
    
    /**
     * @brief Listen socket instance
     */
    GameListenSocket *listenSocketInst;
    
    /**
     * @brief Player map (ID to object)
     */
    std::map<uint32_t, std::shared_ptr<PlayerObject>> m_players;
    
    /**
     * @brief Player map mutex
     */
    std::mutex m_playersMutex;
    
    /**
     * @brief Player handle map (handle to ID)
     */
    std::map<std::string, uint32_t> m_playerHandles;
    
    /**
     * @brief World manager
     */
    WorldManager m_worldManager;
    
    /**
     * @brief Next available object ID
     */
    uint32_t m_nextObjectId;
    
    /**
     * @brief Server start time
     */
    uint32_t m_startTime;
    
    /**
     * @brief Last update time
     */
    uint32_t m_lastUpdate;
    
    /**
     * @brief Update interval in milliseconds
     */
    uint32_t m_updateInterval;
    
    /**
     * @brief Movement update interval in milliseconds
     */
    uint32_t m_movementUpdateInterval;
    
    /**
     * @brief Last movement update time
     */
    uint32_t m_lastMovementUpdate;
    
    /**
     * @brief Player timeout interval in milliseconds
     */
    uint32_t m_playerTimeoutInterval;
    
    /**
     * @brief Last player timeout check time
     */
    uint32_t m_lastTimeoutCheck;
};

/**
 * @brief Global access to the game server singleton
 */
#define sGame GameServer::getSingleton()

#endif // _GAME_SERVER_H_