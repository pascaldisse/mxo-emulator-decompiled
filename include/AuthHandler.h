#ifndef _AUTH_HANDLER_H_
#define _AUTH_HANDLER_H_

#include <Sockets/SocketHandler.h>

/**
 * @brief Authentication socket handler
 * 
 * AuthHandler manages authentication socket connections and events.
 */
class AuthHandler : public SocketHandler {
public:
    /**
     * @brief Default constructor
     */
    AuthHandler();
    
    /**
     * @brief Destructor
     */
    virtual ~AuthHandler();
    
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

private:
    /**
     * @brief Maximum allowed connections
     */
    size_t m_maxConnections;
};

#endif // _AUTH_HANDLER_H_