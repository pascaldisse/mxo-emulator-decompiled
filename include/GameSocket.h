#ifndef _GAME_SOCKET_H_
#define _GAME_SOCKET_H_

#include "ByteBuffer.h"
#include "MessageTypes.h"
#include "LocationVector.h"
#include <Sockets/Socket.h>
#include <Sockets/SocketHandler.h>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <memory>
#include <deque>

/**
 * @brief Game socket
 * 
 * GameSocket handles the UDP connection between the client and the
 * game server, processing game state updates and player commands.
 */
class GameSocket : public UdpSocket {
public:
    /**
     * @brief Socket state enumeration
     */
    enum State {
        STATE_INITIAL = 0,
        STATE_HANDSHAKE = 1,
        STATE_CONNECTED = 2,
        STATE_WORLD_LOADING = 3,
        STATE_IN_WORLD = 4,
        STATE_DISCONNECTING = 5,
        STATE_CLOSED = 6
    };
    
    /**
     * @brief Constructor
     * 
     * @param handler Socket handler
     */
    GameSocket(ISocketHandler& handler);
    
    /**
     * @brief Destructor
     */
    virtual ~GameSocket();
    
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
    void OnRawData(const char* buffer, size_t len, struct sockaddr* sa, socklen_t sa_len);
    
    /**
     * @brief Process a message
     * 
     * @param type Message type
     * @param data Message data
     */
    void ProcessMessage(uint16_t type, ByteBuffer& data);
    
    /**
     * @brief Process a game handshake
     * 
     * @param data Handshake data
     */
    void ProcessGameHandshake(ByteBuffer& data);
    
    /**
     * @brief Send a game session response
     * 
     * @param result Result code
     */
    void SendGameSession(uint16_t result);
    
    /**
     * @brief Process a player movement message
     * 
     * @param data Movement data
     */
    void ProcessPlayerMovement(ByteBuffer& data);
    
    /**
     * @brief Process a player state message
     * 
     * @param data State data
     */
    void ProcessPlayerState(ByteBuffer& data);
    
    /**
     * @brief Process a player command
     * 
     * @param data Command data
     */
    void ProcessPlayerCommand(ByteBuffer& data);
    
    /**
     * @brief Process a region load notification
     * 
     * @param data Notification data
     */
    void ProcessRegionLoad(ByteBuffer& data);
    
    /**
     * @brief Process a jackout request
     * 
     * @param data Request data
     */
    void ProcessJackoutRequest(ByteBuffer& data);
    
    /**
     * @brief Send a jackout response
     * 
     * @param result Result code
     */
    void SendJackoutResponse(uint16_t result);
    
    /**
     * @brief Send a world state update
     * 
     * @param state World state data
     */
    void SendWorldState(const ByteBuffer& state);
    
    /**
     * @brief Send an object create message
     * 
     * @param objectId Object ID
     * @param objectType Object type
     * @param position Object position
     * @param data Object data
     */
    void SendObjectCreate(uint32_t objectId, uint16_t objectType, const LocationVector& position, const ByteBuffer& data);
    
    /**
     * @brief Send an object update message
     * 
     * @param objectId Object ID
     * @param data Update data
     */
    void SendObjectUpdate(uint32_t objectId, const ByteBuffer& data);
    
    /**
     * @brief Send an object destroy message
     * 
     * @param objectId Object ID
     */
    void SendObjectDestroy(uint32_t objectId);
    
    /**
     * @brief Send a chat message
     * 
     * @param sender Sender name
     * @param message Message text
     * @param type Chat message type
     */
    void SendChatMessage(const std::string& sender, const std::string& message, uint8_t type);
    
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
     * @brief Get the current district
     * 
     * @return District ID
     */
    uint8_t GetDistrict() const { return m_district; }
    
    /**
     * @brief Set the current district
     * 
     * @param district District ID
     */
    void SetDistrict(uint8_t district) { m_district = district; }
    
    /**
     * @brief Get the player position
     * 
     * @return Player position
     */
    const LocationVector& GetPosition() const { return m_position; }
    
    /**
     * @brief Set the player position
     * 
     * @param position Player position
     */
    void SetPosition(const LocationVector& position) { m_position = position; }
    
    /**
     * @brief Check if socket is authenticated
     * 
     * @return true if authenticated, false otherwise
     */
    bool IsAuthenticated() const { return m_state >= STATE_CONNECTED; }
    
    /**
     * @brief Check if player is in world
     * 
     * @return true if in world, false otherwise
     */
    bool IsInWorld() const { return m_state >= STATE_IN_WORLD; }
    
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
     * @brief Build a game packet header
     * 
     * @param type Message type
     * @param length Message length
     * @param buffer Buffer to write to
     * @param reliable Is packet reliable
     * @param encrypted Is packet encrypted
     */
    void BuildGameHeader(uint16_t type, uint32_t length, ByteBuffer& buffer, bool reliable, bool encrypted);
    
    /**
     * @brief Send raw data
     * 
     * @param buffer Data buffer
     */
    void SendRawData(const ByteBuffer& buffer);
    
    /**
     * @brief Process acknowledgment
     * 
     * @param ackNum Acknowledgment number
     */
    void ProcessAcknowledgment(uint16_t ackNum);
    
    /**
     * @brief Process reliable packet
     * 
     * @param seqNum Sequence number
     */
    void ProcessReliablePacket(uint16_t seqNum);
    
    /**
     * @brief Send acknowledgment
     * 
     * @param seqNum Sequence number to acknowledge
     */
    void SendAcknowledgment(uint16_t seqNum);
    
    /**
     * @brief Encrypt data
     * 
     * @param data Data to encrypt
     * @return Encrypted data
     */
    ByteBuffer EncryptData(const ByteBuffer& data);
    
    /**
     * @brief Decrypt data
     * 
     * @param data Data to decrypt
     * @return Decrypted data
     */
    ByteBuffer DecryptData(const ByteBuffer& data);
    
    /**
     * @brief Resend unacknowledged packets
     */
    void ResendUnacknowledgedPackets();
    
    /**
     * @brief Check for timeout
     * 
     * @param currentTime Current time in milliseconds
     * @return true if timed out, false otherwise
     */
    bool CheckTimeout(uint32_t currentTime);
    
    /**
     * @brief Send a ping packet
     */
    void SendPing();

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
     * @brief Current district
     */
    uint8_t m_district;
    
    /**
     * @brief Player position
     */
    LocationVector m_position;
    
    /**
     * @brief Incoming data buffer
     */
    ByteBuffer m_recvBuffer;
    
    /**
     * @brief Next sequence number
     */
    uint16_t m_nextSequence;
    
    /**
     * @brief Next expected sequence number
     */
    uint16_t m_expectedSequence;
    
    /**
     * @brief Last acknowledged sequence number
     */
    uint16_t m_lastAcknowledged;
    
    /**
     * @brief Unacknowledged packets map (sequence number to packet data)
     */
    std::map<uint16_t, ByteBuffer> m_unacknowledgedPackets;
    
    /**
     * @brief Received packet history (sequence numbers)
     */
    std::deque<uint16_t> m_receivedPackets;
    
    /**
     * @brief Protocol version
     */
    uint8_t m_protocolVersion;
    
    /**
     * @brief Last activity time
     */
    uint32_t m_lastActivity;
    
    /**
     * @brief Last ping time
     */
    uint32_t m_lastPing;
    
    /**
     * @brief Timeout interval in milliseconds
     */
    uint32_t m_timeoutInterval;
    
    /**
     * @brief Ping interval in milliseconds
     */
    uint32_t m_pingInterval;
    
    /**
     * @brief Resend interval in milliseconds
     */
    uint32_t m_resendInterval;
    
    /**
     * @brief Last resend time
     */
    uint32_t m_lastResend;
    
    /**
     * @brief Socket mutex
     */
    std::mutex m_socketMutex;
    
    /**
     * @brief Client address
     */
    struct sockaddr_in m_clientAddr;
    
    /**
     * @brief Client address length
     */
    socklen_t m_clientAddrLen;
};

#endif // _GAME_SOCKET_H_