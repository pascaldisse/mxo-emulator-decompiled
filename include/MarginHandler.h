#ifndef _MARGIN_HANDLER_H_
#define _MARGIN_HANDLER_H_

#include <Sockets/SocketHandler.h>

/**
 * @brief Margin socket handler
 * 
 * MarginHandler manages margin socket connections and events.
 */
class MarginHandler : public SocketHandler {
public:
    /**
     * @brief Default constructor
     */
    MarginHandler();
    
    /**
     * @brief Destructor
     */
    virtual ~MarginHandler();
    
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
    class MarginSocket* FindSocketByPlayerId(uint32_t playerId);
    
    /**
     * @brief Find a socket by account ID
     * 
     * @param accountId Account ID to find
     * @return Pointer to socket, or nullptr if not found
     */
    class MarginSocket* FindSocketByAccountId(uint32_t accountId);
    
    /**
     * @brief Send a mission update to a player
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @return true if sent, false otherwise
     */
    bool SendMissionUpdateToPlayer(uint32_t playerId, uint32_t missionId);
    
    /**
     * @brief Send a mission complete notification to a player
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @return true if sent, false otherwise
     */
    bool SendMissionCompleteToPlayer(uint32_t playerId, uint32_t missionId);

private:
    /**
     * @brief Maximum allowed connections
     */
    size_t m_maxConnections;
};

#endif // _MARGIN_HANDLER_H_