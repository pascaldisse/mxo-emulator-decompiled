#ifndef _PLAYER_OBJECT_H_
#define _PLAYER_OBJECT_H_

#include "LocationVector.h"
#include "MessageTypes.h"
#include <string>
#include <queue>
#include <list>
#include <boost/function.hpp>

/**
 * @brief Represents a player character in the game
 * 
 * PlayerObject manages a player character entity, handling its state,
 * movement, interactions, and messaging.
 */
class PlayerObject
{
public:
    /**
     * @brief Exception thrown when a character is not found
     */
    class CharacterNotFound {};

    /**
     * @brief Constructor
     * 
     * @param parent Reference to the owning GameClient
     * @param charUID Unique ID of the character
     */
    PlayerObject(class GameClient &parent, uint64_t charUID);
    
    /**
     * @brief Destructor
     */
    ~PlayerObject();

    /**
     * @brief Initialize the character in the world
     */
    void InitializeWorld();
    
    /**
     * @brief Spawn the character in the world
     */
    void SpawnSelf();
    
    /**
     * @brief Populate the world around the character
     */
    void PopulateWorld();

    /**
     * @brief Set the game object ID
     * 
     * @param theGoId The game object ID to assign
     */
    void initGoId(uint32_t theGoId);
    
    /**
     * @brief Handle a state update message
     * 
     * @param srcData The state update data
     */
    void HandleStateUpdate(ByteBuffer &srcData);
    
    /**
     * @brief Handle a command message
     * 
     * @param srcCmd The command data
     */
    void HandleCommand(ByteBuffer &srcCmd);

    /**
     * @brief Get the character's handle (name)
     * 
     * @return The character handle
     */
    std::string getHandle() const { return m_handle; }
    
    /**
     * @brief Get the character's first name
     * 
     * @return The first name
     */
    std::string getFirstName() const { return m_firstName; }
    
    /**
     * @brief Get the character's last name
     * 
     * @return The last name
     */
    std::string getLastName() const { return m_lastName; }
    
    /**
     * @brief Get the character's background
     * 
     * @return The background text
     */
    std::string getBackground() const { return m_background; }
    
    /**
     * @brief Set the character's background
     * 
     * @param newBackground The new background text
     * @return true if successful, false otherwise
     */
    bool setBackground(std::string newBackground);

    /**
     * @brief Get the character's experience
     * 
     * @return Experience points
     */
    uint64_t getExperience() const { return m_exp; }
    
    /**
     * @brief Get the character's information (currency)
     * 
     * @return Information amount
     */
    uint64_t getInformation() const { return m_cash; }
    
    /**
     * @brief Get the character's position
     * 
     * @return Current position
     */
    LocationVector getPosition() const { return m_pos; }
    
    /**
     * @brief Set the character's position
     * 
     * @param newPos New position
     */
    void setPosition(const LocationVector& newPos) { m_pos = newPos; }
    
    /**
     * @brief Get the character's district
     * 
     * @return District ID
     */
    uint8_t getDistrict() const { return m_district; }
    
    /**
     * @brief Set the character's district
     * 
     * @param newDistrict New district ID
     */
    void setDistrict(uint8_t newDistrict) { m_district = newDistrict; }
    
    /**
     * @brief Get the character's RSI data (appearance)
     * 
     * @param outputBuf Buffer to receive the data
     * @param maxBufLen Maximum buffer length
     * @return Number of bytes written
     */
    uint8_t getRsiData(byte* outputBuf, size_t maxBufLen) const;
    
    /**
     * @brief Get the character's current health
     * 
     * @return Current health
     */
    uint16_t getCurrentHealth() const { return m_healthC; }
    
    /**
     * @brief Get the character's maximum health
     * 
     * @return Maximum health
     */
    uint16_t getMaximumHealth() const { return m_healthM; }
    
    /**
     * @brief Get the character's current inner strength
     * 
     * @return Current inner strength
     */
    uint16_t getCurrentIS() const { return m_innerStrC; }
    
    /**
     * @brief Get the character's maximum inner strength
     * 
     * @return Maximum inner strength
     */
    uint16_t getMaximumIS() const { return m_innerStrM; }
    
    /**
     * @brief Get the character's profession
     * 
     * @return Profession ID
     */
    uint32_t getProfession() const { return m_prof; }
    
    /**
     * @brief Get the character's level
     * 
     * @return Character level
     */
    uint8_t getLevel() const { return m_lvl; }
    
    /**
     * @brief Get the character's alignment
     * 
     * @return Alignment value
     */
    uint8_t getAlignment() const { return m_alignment; }
    
    /**
     * @brief Get the character's PvP flag
     * 
     * @return PvP flag status
     */
    bool getPvpFlag() const { return m_pvpflag; }

    /**
     * @brief Get the character's current animation
     * 
     * @return Animation ID
     */
    uint8_t getCurrentAnimation() const { return m_currAnimation; }
    
    /**
     * @brief Get the character's current mood
     * 
     * @return Mood ID
     */
    uint8_t getCurrentMood() const { return m_currMood; }

    /**
     * @brief Get the owning game client
     * 
     * @return Reference to the game client
     */
    class GameClient& getClient() { return m_parent; }
    
    /**
     * @brief Get the current state packets
     * 
     * @return Vector of state message packets
     */
    std::vector<msgBaseClassPtr> getCurrentStatePackets();

    /**
     * @brief Update the character
     * 
     * Process any pending events and update the character state.
     */
    void Update();

private:
    // RPC handler type
    typedef void (PlayerObject::*RPCHandler)(ByteBuffer &srcCmd);

    // RPC handlers
    void RPC_NullHandle(ByteBuffer &srcCmd);
    void RPC_HandleReadyForSpawn(ByteBuffer &srcCmd);
    void RPC_HandleChat(ByteBuffer &srcCmd);
    void RPC_HandleWhisper(ByteBuffer &srcCmd);
    void RPC_HandleStopAnimation(ByteBuffer &srcCmd);
    void RPC_HandleStartAnimtion(ByteBuffer &srcCmd);
    void RPC_HandleChangeMood(ByteBuffer &srcCmd);
    void RPC_HandlePerformEmote(ByteBuffer &srcCmd);
    void RPC_HandleDynamicObjInteraction(ByteBuffer &srcCmd);
    void RPC_HandleStaticObjInteraction(ByteBuffer &srcCmd);
    void RPC_HandleJump(ByteBuffer &srcCmd);
    void RPC_HandleRegionLoadedNotification(ByteBuffer &srcCmd);
    void RPC_HandleReadyForWorldChange(ByteBuffer &srcCmd);
    void RPC_HandleWho(ByteBuffer &srcCmd);
    void RPC_HandleWhereAmI(ByteBuffer &srcCmd);
    void RPC_HandleGetPlayerDetails(ByteBuffer &srcCmd);
    void RPC_HandleGetBackground(ByteBuffer &srcCmd);
    void RPC_HandleSetBackground(ByteBuffer &srcCmd);
    void RPC_HandleHardlineTeleport(ByteBuffer &srcCmd);
    void RPC_HandleObjectSelected(ByteBuffer &srcCmd);
    void RPC_HandleJackoutRequest(ByteBuffer &srcCmd);
    void RPC_HandleJackoutFinished(ByteBuffer &srcCmd);

    // RPC Handler maps
    std::map<uint8_t, RPCHandler> m_RPCbyte;
    std::map<uint16_t, RPCHandler> m_RPCshort;

private:
    void loadFromDB(bool updatePos = false);
    void checkAndStore();
    void saveDataToDB();
    void setOnlineStatus(bool isOnline);

    typedef enum
    {
        EVENT_JACKOUT
    } eventType;

    typedef boost::function<void(void)> eventFunc;

    void addEvent(eventType type, eventFunc func, float activationTime);
    size_t cancelEvents(eventType type);

    struct eventStruct
    {
        eventStruct(eventType _type, eventFunc _func, float _fireTime) : type(_type), func(_func), fireTime(_fireTime) {}
        eventType type;
        eventFunc func;
        float fireTime;
    };

    std::list<eventStruct> m_events;

    void jackoutEvent();

    void ParseAdminCommand(std::string theCmd);
    void ParsePlayerCommand(std::string theCmd);
    void GoAhead(double distanceToGo);
    void UpdateAppearance();
    class GameClient &m_parent;
    
    // Player info
    uint64_t m_characterUID;
    std::string m_handle;
    std::string m_firstName;
    std::string m_lastName;
    std::string m_background;

    uint32_t m_goId;
    uint64_t m_exp, m_cash;
    uint8_t m_district;
    LocationVector m_pos, m_savedPos;
    std::shared_ptr<class RsiData> m_rsi;
    uint16_t m_healthC, m_healthM, m_innerStrC, m_innerStrM;
    uint32_t m_prof;
    uint8_t m_lvl, m_alignment;
    bool m_pvpflag;
    uint32_t testCount;

    bool m_spawnedInWorld;
    std::queue<msgBaseClassPtr> m_sendAfterSpawn;
    bool m_worldPopulated;

    uint32_t m_lastStore;
    uint32_t m_storeCntr;

    uint8_t m_currAnimation;
    uint8_t m_currMood;

    uint8_t m_emoteCounter;

    bool m_isAdmin;
};

#endif // _PLAYER_OBJECT_H_