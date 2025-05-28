#ifndef _MESSAGE_TYPES_H_
#define _MESSAGE_TYPES_H_

#include <memory>
#include "ByteBuffer.h"

/**
 * @brief Base class for all message types
 */
class MessageBase {
public:
    /**
     * @brief Virtual destructor
     */
    virtual ~MessageBase() {}
    
    /**
     * @brief Serialize the message to a ByteBuffer
     * 
     * @param buffer ByteBuffer to serialize to
     */
    virtual void Serialize(ByteBuffer& buffer) const = 0;
    
    /**
     * @brief Deserialize the message from a ByteBuffer
     * 
     * @param buffer ByteBuffer to deserialize from
     * @return true if successful, false otherwise
     */
    virtual bool Deserialize(ByteBuffer& buffer) = 0;
    
    /**
     * @brief Get the message type
     * 
     * @return Message type identifier
     */
    virtual uint16_t GetType() const = 0;
};

/**
 * @brief Shared pointer type for message objects
 */
typedef std::shared_ptr<MessageBase> msgBaseClassPtr;

/**
 * @brief Authentication message types
 */
enum AuthMessageTypes {
    // Authentication messages (0x0001 - 0x00FF)
    MSG_AUTH_CHALLENGE            = 0x0001,
    MSG_AUTH_RESPONSE             = 0x0002,
    MSG_AUTH_RESULT               = 0x0003,
    MSG_CHAR_LIST_REQUEST         = 0x0004,
    MSG_CHAR_LIST_RESPONSE        = 0x0005,
    MSG_CHAR_CREATE_REQUEST       = 0x0006,
    MSG_CHAR_CREATE_RESPONSE      = 0x0007,
    MSG_CHAR_DELETE_REQUEST       = 0x0008,
    MSG_CHAR_DELETE_RESPONSE      = 0x0009,
    MSG_WORLD_LIST_REQUEST        = 0x000A,
    MSG_WORLD_LIST_RESPONSE       = 0x000B,
    MSG_CHAR_SELECT_REQUEST       = 0x000C,
    MSG_CHAR_SELECT_RESPONSE      = 0x000D
};

/**
 * @brief Game server message types
 */
enum GameMessageTypes {
    // Game server messages (0x1000 - 0x1FFF)
    MSG_GAME_HANDSHAKE            = 0x1001,
    MSG_GAME_SESSION              = 0x1002,
    MSG_PLAYER_MOVEMENT           = 0x1003,
    MSG_PLAYER_STATE              = 0x1004,
    MSG_WORLD_STATE               = 0x1005,
    MSG_OBJECT_CREATE             = 0x1006,
    MSG_OBJECT_UPDATE             = 0x1007,
    MSG_OBJECT_DESTROY            = 0x1008,
    MSG_CHAT_MESSAGE              = 0x1009,
    MSG_PLAYER_COMMAND            = 0x100A,
    MSG_REGION_LOAD               = 0x100B,
    MSG_JACKOUT_REQUEST           = 0x100C,
    MSG_JACKOUT_RESPONSE          = 0x100D,
    
    // Player state flags
    PLAYER_STATE_COMBAT           = 0x0001,
    PLAYER_STATE_SITTING          = 0x0002,
    PLAYER_STATE_DEAD             = 0x0004,
    PLAYER_STATE_INVISIBLE        = 0x0008,
    PLAYER_STATE_PVP_ENABLED      = 0x0010,
    PLAYER_STATE_RUNNING          = 0x0020,
    PLAYER_STATE_STEALTHED        = 0x0040,
    PLAYER_STATE_AFK              = 0x0080
};

/**
 * @brief Margin server message types
 */
enum MarginMessageTypes {
    // Margin server messages (0x2000 - 0x2FFF)
    MSG_MISSION_LIST_REQUEST      = 0x2001,
    MSG_MISSION_LIST_RESPONSE     = 0x2002,
    MSG_MISSION_ACCEPT            = 0x2003,
    MSG_MISSION_UPDATE            = 0x2004,
    MSG_MISSION_COMPLETE          = 0x2005,
    MSG_DIALOGUE_REQUEST          = 0x2006,
    MSG_DIALOGUE_RESPONSE         = 0x2007,
    MSG_DIALOGUE_CHOICE           = 0x2008
};

/**
 * @brief Player command types
 */
enum PlayerCommandTypes {
    // Byte commands (0x00 - 0xFF)
    CMD_READY_FOR_SPAWN           = 0x01,
    CMD_CHAT                      = 0x02,
    CMD_WHISPER                   = 0x03,
    CMD_STOP_ANIMATION            = 0x04,
    CMD_START_ANIMATION           = 0x05,
    CMD_CHANGE_MOOD               = 0x06,
    CMD_PERFORM_EMOTE             = 0x07,
    CMD_DYNAMIC_OBJ_INTERACTION   = 0x08,
    CMD_STATIC_OBJ_INTERACTION    = 0x09,
    CMD_JUMP                      = 0x0A,
    CMD_REGION_LOADED             = 0x0B,
    CMD_READY_FOR_WORLD_CHANGE    = 0x0C,
    CMD_WHO                       = 0x0D,
    CMD_WHERE_AM_I                = 0x0E,
    CMD_GET_PLAYER_DETAILS        = 0x0F,
    CMD_GET_BACKGROUND            = 0x10,
    CMD_SET_BACKGROUND            = 0x11,
    CMD_HARDLINE_TELEPORT         = 0x12,
    CMD_OBJECT_SELECTED           = 0x13,
    CMD_JACKOUT_REQUEST           = 0x14,
    CMD_JACKOUT_FINISHED          = 0x15,
    
    // Short commands (0x0100 - 0xFFFF)
    CMD_ABILITY_USE               = 0x0100,
    CMD_TRADE_REQUEST             = 0x0101,
    CMD_TRADE_ACCEPT              = 0x0102,
    CMD_TRADE_DECLINE             = 0x0103,
    CMD_TRADE_CANCEL              = 0x0104,
    CMD_TRADE_ADD_ITEM            = 0x0105,
    CMD_TRADE_REMOVE_ITEM         = 0x0106,
    CMD_TRADE_SET_INFO            = 0x0107,
    CMD_TRADE_CONFIRM             = 0x0108,
    CMD_GROUP_INVITE              = 0x0109,
    CMD_GROUP_ACCEPT              = 0x010A,
    CMD_GROUP_DECLINE             = 0x010B,
    CMD_GROUP_LEAVE               = 0x010C,
    CMD_GROUP_KICK                = 0x010D,
    CMD_GROUP_PROMOTE             = 0x010E,
    CMD_GROUP_DISBAND             = 0x010F
};

/**
 * @brief Chat message types
 */
enum ChatMessageTypes {
    CHAT_SAY                      = 0x00,
    CHAT_YELL                     = 0x01,
    CHAT_WHISPER                  = 0x02,
    CHAT_GROUP                    = 0x03,
    CHAT_FACTION                  = 0x04,
    CHAT_SYSTEM                   = 0x05,
    CHAT_EMOTE                    = 0x06,
    CHAT_OOC                      = 0x07,
    CHAT_BROADCAST                = 0x08
};

/**
 * @brief Authentication result codes
 */
enum AuthResultCodes {
    AUTH_SUCCESS                  = 0x00,
    AUTH_INVALID_CREDENTIALS      = 0x01,
    AUTH_ACCOUNT_BANNED           = 0x02,
    AUTH_SERVER_FULL              = 0x03,
    AUTH_ALREADY_LOGGED_IN        = 0x04,
    AUTH_INVALID_CLIENT_VERSION   = 0x05,
    AUTH_INTERNAL_ERROR           = 0x06,
    AUTH_ACCOUNT_SUSPENDED        = 0x07,
    AUTH_NO_ACCESS                = 0x08,
    AUTH_MAINTENANCE              = 0x09
};

/**
 * @brief Object types
 */
enum ObjectTypes {
    OBJECT_TYPE_NONE              = 0x0000,
    OBJECT_TYPE_PLAYER            = 0x0001,
    OBJECT_TYPE_NPC               = 0x0002,
    OBJECT_TYPE_ITEM              = 0x0003,
    OBJECT_TYPE_CONTAINER         = 0x0004,
    OBJECT_TYPE_HARDLINE          = 0x0005,
    OBJECT_TYPE_DOOR              = 0x0006,
    OBJECT_TYPE_COMPUTER          = 0x0007,
    OBJECT_TYPE_MISSION_GIVER     = 0x0008,
    OBJECT_TYPE_VENDOR            = 0x0009,
    OBJECT_TYPE_TRAINER           = 0x000A,
    OBJECT_TYPE_INTERACTIVE       = 0x000B,
    OBJECT_TYPE_ELEVATOR          = 0x000C,
    OBJECT_TYPE_PORTAL            = 0x000D
};

/**
 * @brief Animation IDs
 */
enum AnimationIds {
    ANIM_STAND                    = 0x00,
    ANIM_WALK                     = 0x01,
    ANIM_RUN                      = 0x02,
    ANIM_JUMP                     = 0x03,
    ANIM_COMBAT_IDLE              = 0x04,
    ANIM_COMBAT_WALK              = 0x05,
    ANIM_COMBAT_RUN               = 0x06,
    ANIM_COMBAT_JUMP              = 0x07,
    ANIM_SIT                      = 0x08,
    ANIM_KNEEL                    = 0x09,
    ANIM_DEATH                    = 0x0A,
    ANIM_WAVE                     = 0x0B,
    ANIM_BOW                      = 0x0C,
    ANIM_CLAP                     = 0x0D,
    ANIM_DANCE                    = 0x0E,
    ANIM_LAUGH                    = 0x0F,
    ANIM_POINT                    = 0x10,
    ANIM_SHRUG                    = 0x11,
    ANIM_TAUNT                    = 0x12,
    ANIM_MEDITATE                 = 0x13
};

/**
 * @brief Mood IDs
 */
enum MoodIds {
    MOOD_NEUTRAL                  = 0x00,
    MOOD_HAPPY                    = 0x01,
    MOOD_SAD                      = 0x02,
    MOOD_ANGRY                    = 0x03,
    MOOD_SURPRISED                = 0x04,
    MOOD_AFRAID                   = 0x05,
    MOOD_DISGUSTED                = 0x06,
    MOOD_BORED                    = 0x07,
    MOOD_DETERMINED               = 0x08,
    MOOD_CONFUSED                 = 0x09,
    MOOD_SUSPICIOUS               = 0x0A
};

/**
 * @brief District IDs
 */
enum DistrictIds {
    DISTRICT_RICHLAND             = 0x01,
    DISTRICT_DOWNTOWN             = 0x02,
    DISTRICT_WESTVIEW             = 0x03,
    DISTRICT_INTERNATIONAL        = 0x04,
    DISTRICT_UENO                 = 0x05,
    DISTRICT_STAMOS               = 0x06,
    DISTRICT_TABOR                = 0x07,
    DISTRICT_EDGEWATER            = 0x08,
    DISTRICT_GRACY                = 0x09,
    DISTRICT_HISTORIC             = 0x0A,
    DISTRICT_CENTER               = 0x0B,
    DISTRICT_KEDEMOTH             = 0x0C
};

/**
 * @brief Profession IDs
 */
enum ProfessionIds {
    PROFESSION_NONE               = 0x00,
    PROFESSION_CODER              = 0x01,
    PROFESSION_HACKER             = 0x02,
    PROFESSION_OPERATIVE          = 0x03,
    PROFESSION_REDPILL            = 0x04
};

/**
 * @brief Alignment IDs
 */
enum AlignmentIds {
    ALIGNMENT_NEUTRAL             = 0x00,
    ALIGNMENT_ZION                = 0x01,
    ALIGNMENT_MACHINE             = 0x02,
    ALIGNMENT_MEROVINGIAN         = 0x03
};

/**
 * @brief Network packet flags
 */
enum PacketFlags {
    PACKET_FLAG_RELIABLE          = 0x01,
    PACKET_FLAG_ENCRYPTED         = 0x02,
    PACKET_FLAG_COMPRESSED        = 0x04,
    PACKET_FLAG_FRAGMENT          = 0x08
};

#endif // _MESSAGE_TYPES_H_