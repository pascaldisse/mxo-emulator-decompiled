#ifndef _AUTH_SOCKET_H_
#define _AUTH_SOCKET_H_

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
 * @brief Authentication socket
 * 
 * AuthSocket handles the TCP connection between the client and the
 * authentication server, processing login requests and character selection.
 */
class AuthSocket : public TcpSocket {
public:
    /**
     * @brief Socket state enumeration
     */
    enum State {
        STATE_INITIAL = 0,
        STATE_CHALLENGE_SENT = 1,
        STATE_AUTHENTICATING = 2,
        STATE_AUTHENTICATED = 3,
        STATE_CHARACTER_LIST = 4,
        STATE_CHARACTER_SELECTED = 5,
        STATE_CLOSED = 6
    };
    
    /**
     * @brief Constructor
     * 
     * @param handler Socket handler
     */
    AuthSocket(ISocketHandler& handler);
    
    /**
     * @brief Destructor
     */
    virtual ~AuthSocket();
    
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
     * @brief Send an authentication challenge
     */
    void SendAuthChallenge();
    
    /**
     * @brief Process an authentication response
     * 
     * @param data Response data
     */
    void ProcessAuthResponse(ByteBuffer& data);
    
    /**
     * @brief Send an authentication result
     * 
     * @param result Result code
     * @param sessionKey Session key (if successful)
     */
    void SendAuthResult(uint16_t result, const std::string& sessionKey = "");
    
    /**
     * @brief Process a character list request
     * 
     * @param data Request data
     */
    void ProcessCharListRequest(ByteBuffer& data);
    
    /**
     * @brief Send a character list response
     */
    void SendCharListResponse();
    
    /**
     * @brief Process a character create request
     * 
     * @param data Request data
     */
    void ProcessCharCreateRequest(ByteBuffer& data);
    
    /**
     * @brief Send a character create response
     * 
     * @param result Result code
     * @param errorMessage Error message (if failed)
     */
    void SendCharCreateResponse(uint16_t result, const std::string& errorMessage = "");
    
    /**
     * @brief Process a character delete request
     * 
     * @param data Request data
     */
    void ProcessCharDeleteRequest(ByteBuffer& data);
    
    /**
     * @brief Send a character delete response
     * 
     * @param result Result code
     * @param errorMessage Error message (if failed)
     */
    void SendCharDeleteResponse(uint16_t result, const std::string& errorMessage = "");
    
    /**
     * @brief Process a world list request
     * 
     * @param data Request data
     */
    void ProcessWorldListRequest(ByteBuffer& data);
    
    /**
     * @brief Send a world list response
     */
    void SendWorldListResponse();
    
    /**
     * @brief Process a character select request
     * 
     * @param data Request data
     */
    void ProcessCharSelectRequest(ByteBuffer& data);
    
    /**
     * @brief Send a character select response
     * 
     * @param result Result code
     * @param errorMessage Error message (if failed)
     */
    void SendCharSelectResponse(uint16_t result, const std::string& errorMessage = "");
    
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
     * @brief Get the account ID
     * 
     * @return Account ID
     */
    uint32_t GetAccountId() const { return m_accountId; }
    
    /**
     * @brief Get the account name
     * 
     * @return Account name
     */
    const std::string& GetAccountName() const { return m_accountName; }
    
    /**
     * @brief Get the selected character ID
     * 
     * @return Character ID
     */
    uint64_t GetSelectedCharacterId() const { return m_selectedCharacterId; }
    
    /**
     * @brief Get the selected world ID
     * 
     * @return World ID
     */
    uint16_t GetSelectedWorldId() const { return m_selectedWorldId; }
    
    /**
     * @brief Get the session key
     * 
     * @return Session key
     */
    const std::string& GetSessionKey() const { return m_sessionKey; }

private:
    /**
     * @brief Generate a random challenge string
     * 
     * @param length Challenge length
     * @return Random challenge string
     */
    std::string GenerateChallenge(uint32_t length);
    
    /**
     * @brief Generate a random session key
     * 
     * @param length Session key length
     * @return Random session key
     */
    std::string GenerateSessionKey(uint32_t length);
    
    /**
     * @brief Encrypt data with RSA
     * 
     * @param data Data to encrypt
     * @return Encrypted data
     */
    std::string EncryptRSA(const std::string& data);
    
    /**
     * @brief Decrypt data with RSA
     * 
     * @param data Data to decrypt
     * @return Decrypted data
     */
    std::string DecryptRSA(const std::string& data);
    
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
     * @brief Socket state
     */
    State m_state;
    
    /**
     * @brief Account ID
     */
    uint32_t m_accountId;
    
    /**
     * @brief Account name
     */
    std::string m_accountName;
    
    /**
     * @brief Selected character ID
     */
    uint64_t m_selectedCharacterId;
    
    /**
     * @brief Selected world ID
     */
    uint16_t m_selectedWorldId;
    
    /**
     * @brief Challenge string
     */
    std::string m_challenge;
    
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

#endif // _AUTH_SOCKET_H_