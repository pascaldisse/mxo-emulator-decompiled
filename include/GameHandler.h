#ifndef _GAME_HANDLER_H_
#define _GAME_HANDLER_H_

#include <Sockets/SocketHandler.h>

/**
 * @brief Game socket handler
 * 
 * GameHandler manages game socket connections and events.
 */
class GameHandler : public SocketHandler {
public:
    /**
     * @brief Default constructor
     */
    GameHandler();
    
    /**
     * @brief Destructor
     */
    virtual ~GameHandler();
    
    /**
     * @brief Called when a socket is opened
     * 
     * @param socket The socket that was opened
     */
    void OnOpen(Socket *socket);
    
    /**
     * @brief Called when a socket is closed
     * 
     * @param socket The socket that was closed
     */
    void OnClose(Socket *socket);
    
    /**
     * @brief Called when an exception occurs
     * 
     * @param socket The socket that had an exception
     * @param error Error message
     */
    void OnException(Socket *socket, const std::string& error);
    
    /**
     * @brief Called when a timeout occurs
     * 
     * @param socket The socket that timed out
     */
    void OnTimeout(Socket *socket);
    
    /**
     * @brief Get the count of active connections
     * 
     * @return Number of active connections
     */
    size_t GetConnectionCount() const;
    
    /**
     * @brief Get the maximum allowed connections
     * 
     * @return Maximum allowed connections
     */
    size_t GetMaxConnections() const;
    
    /**
     * @brief Set the maximum allowed connections
     * 
     * @param maxConnections Maximum allowed connections
     */
    void SetMaxConnections(size_t maxConnections);
    
    /**
     * @brief Update all sockets
     * 
     * @param diff Time difference since last update in milliseconds
     */
    void Update(uint32_t diff);
    
    /**
     * @brief Find a socket by player ID
     * 
     * @param playerId Player ID to find
     * @return Pointer to socket, or nullptr if not found
     */
    class GameSocket* FindSocketByPlayerId(uint32_t playerId);
    
    /**
     * @brief Find a socket by account ID
     * 
     * @param accountId Account ID to find
     * @return Pointer to socket, or nullptr if not found
     */
    class GameSocket* FindSocketByAccountId(uint32_t accountId);
    
    /**
     * @brief Broadcast a message to all sockets in a district
     * 
     * @param district District ID
     * @param buffer Message buffer
     * @param exceptId Player ID to exclude (0 = none)
     */
    void BroadcastToDistrict(uint8_t district, const ByteBuffer& buffer, uint32_t exceptId = 0);
    
    /**
     * @brief Broadcast a message to all sockets
     * 
     * @param buffer Message buffer
     * @param exceptId Player ID to exclude (0 = none)
     */
    void BroadcastToAll(const ByteBuffer& buffer, uint32_t exceptId = 0);

private:
    /**
     * @brief Maximum allowed connections
     */
    size_t m_maxConnections;
};

#endif // _GAME_HANDLER_H_