#ifndef _MARGIN_SOCKET_H_
#define _MARGIN_SOCKET_H_

#include "ByteBuffer.h"
#include "MessageTypes.h"
#include <Sockets/Socket.h>
#include <Sockets/SocketHandler.h>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <memory>

/**
 * @brief Margin socket
 * 
 * MarginSocket handles the TCP connection between the client and the
 * margin server, processing mission and dialogue requests.
 */
class MarginSocket : public TcpSocket {
public:
    /**
     * @brief Socket state enumeration
     */
    enum State {
        STATE_INITIAL = 0,
        STATE_CONNECTED = 1,
        STATE_AUTHENTICATED = 2,
        STATE_CLOSED = 3
    };
    
    /**
     * @brief Constructor
     * 
     * @param handler Socket handler
     */
    MarginSocket(ISocketHandler& handler);
    
    /**
     * @brief Destructor
     */
    virtual ~MarginSocket();
    
    /**
     * @brief Called when socket is open
     */
    void OnAccept();
    
    /**
     * @brief Called when socket is closed
     */
    void OnDisconnect();
    
    /**
     * @brief Called when data is received
     */
    void OnRawData(const char* buffer, size_t len);
    
    /**
     * @brief Process a message
     * 
     * @param type Message type
     * @param data Message data
     */
    void ProcessMessage(uint16_t type, ByteBuffer& data);
    
    /**
     * @brief Process a mission list request
     * 
     * @param data Request data
     */
    void ProcessMissionListRequest(ByteBuffer& data);
    
    /**
     * @brief Send a mission list response
     * 
     * @param playerId Player ID
     */
    void SendMissionListResponse(uint32_t playerId);
    
    /**
     * @brief Process a mission accept
     * 
     * @param data Request data
     */
    void ProcessMissionAccept(ByteBuffer& data);
    
    /**
     * @brief Send a mission update
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     */
    void SendMissionUpdate(uint32_t playerId, uint32_t missionId);
    
    /**
     * @brief Send a mission complete notification
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     */
    void SendMissionComplete(uint32_t playerId, uint32_t missionId);
    
    /**
     * @brief Process a dialogue request
     * 
     * @param data Request data
     */
    void ProcessDialogueRequest(ByteBuffer& data);
    
    /**
     * @brief Send a dialogue response
     * 
     * @param playerId Player ID
     * @param dialogueId Dialogue ID
     */
    void SendDialogueResponse(uint32_t playerId, uint32_t dialogueId);
    
    /**
     * @brief Process a dialogue choice
     * 
     * @param data Request data
     */
    void ProcessDialogueChoice(ByteBuffer& data);
    
    /**
     * @brief Get the socket state
     * 
     * @return Current socket state
     */
    State GetState() const { return m_state; }
    
    /**
     * @brief Set the socket state
     * 
     * @param state New socket state
     */
    void SetState(State state) { m_state = state; }
    
    /**
     * @brief Get the player ID
     * 
     * @return Player ID
     */
    uint32_t GetPlayerId() const { return m_playerId; }
    
    /**
     * @brief Set the player ID
     * 
     * @param playerId Player ID
     */
    void SetPlayerId(uint32_t playerId) { m_playerId = playerId; }
    
    /**
     * @brief Get the account ID
     * 
     * @return Account ID
     */
    uint32_t GetAccountId() const { return m_accountId; }
    
    /**
     * @brief Set the account ID
     * 
     * @param accountId Account ID
     */
    void SetAccountId(uint32_t accountId) { m_accountId = accountId; }
    
    /**
     * @brief Get the session key
     * 
     * @return Session key
     */
    const std::string& GetSessionKey() const { return m_sessionKey; }
    
    /**
     * @brief Set the session key
     * 
     * @param sessionKey Session key
     */
    void SetSessionKey(const std::string& sessionKey) { m_sessionKey = sessionKey; }
    
    /**
     * @brief Check if socket is authenticated
     * 
     * @return true if authenticated, false otherwise
     */
    bool IsAuthenticated() const { return m_state >= STATE_AUTHENTICATED; }
    
    /**
     * @brief Update the socket
     * 
     * @param diff Time difference since last update in milliseconds
     */
    void Update(uint32_t diff);

private:
    /**
     * @brief Build a common message header
     * 
     * @param type Message type
     * @param length Message length
     * @param buffer Buffer to write to
     */
    void BuildHeader(uint16_t type, uint32_t length, ByteBuffer& buffer);
    
    /**
     * @brief Send raw data
     * 
     * @param buffer Data buffer
     */
    void SendRawData(const ByteBuffer& buffer);
    
    /**
     * @brief Authenticate the session
     * 
     * @param sessionKey Session key
     * @param accountId Account ID
     * @param playerId Player ID
     * @return true if authenticated, false otherwise
     */
    bool Authenticate(const std::string& sessionKey, uint32_t accountId, uint32_t playerId);
    
    /**
     * @brief Check for timeout
     * 
     * @param currentTime Current time in milliseconds
     * @return true if timed out, false otherwise
     */
    bool CheckTimeout(uint32_t currentTime);

    /**
     * @brief Socket state
     */
    State m_state;
    
    /**
     * @brief Player ID
     */
    uint32_t m_playerId;
    
    /**
     * @brief Account ID
     */
    uint32_t m_accountId;
    
    /**
     * @brief Session key
     */
    std::string m_sessionKey;
    
    /**
     * @brief Incoming data buffer
     */
    ByteBuffer m_recvBuffer;
    
    /**
     * @brief Protocol version
     */
    uint8_t m_protocolVersion;
    
    /**
     * @brief Last activity time
     */
    uint32_t m_lastActivity;
    
    /**
     * @brief Timeout interval in milliseconds
     */
    uint32_t m_timeoutInterval;
    
    /**
     * @brief Socket mutex
     */
    std::mutex m_socketMutex;
};

#endif // _MARGIN_SOCKET_H_