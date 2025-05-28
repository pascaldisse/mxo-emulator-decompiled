#ifndef _DIALOGUE_MANAGER_H_
#define _DIALOGUE_MANAGER_H_

#include "ByteBuffer.h"
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <memory>

/**
 * @brief Dialogue option structure
 */
struct DialogueOption {
    uint32_t id;                   ///< Option ID
    std::string text;              ///< Option text
    uint32_t nextDialogueId;       ///< Next dialogue ID if selected
    bool endConversation;          ///< Does this option end the conversation
    uint32_t requiredMissionId;    ///< Required mission ID (0 = none)
    uint8_t requiredMissionState;  ///< Required mission state (0=not started, 1=in progress, 2=completed)
    uint8_t requiredLevel;         ///< Required player level (0 = any)
    uint8_t requiredFaction;       ///< Required faction (0 = any)
    uint32_t requiredSkillId;      ///< Required skill ID (0 = none)
    uint8_t requiredSkillLevel;    ///< Required skill level
};

/**
 * @brief Dialogue action structure
 */
struct DialogueAction {
    enum ActionType {
        NONE = 0,
        START_MISSION = 1,
        COMPLETE_MISSION = 2,
        GIVE_ITEM = 3,
        TAKE_ITEM = 4,
        TELEPORT = 5,
        OPEN_SHOP = 6,
        TRAIN_SKILL = 7,
        SET_FACTION = 8
    };
    
    ActionType type;                ///< Action type
    uint32_t value;                 ///< Action value
    uint32_t secondaryValue;        ///< Secondary action value
    std::string actionText;         ///< Text shown when action occurs
};

/**
 * @brief Dialogue entry structure
 */
struct DialogueEntry {
    uint32_t id;                   ///< Dialogue ID
    uint32_t npcId;                ///< NPC ID (speaker)
    std::string text;              ///< Dialogue text
    uint8_t npcEmotion;            ///< NPC emotion (0-10)
    uint8_t npcAnimation;          ///< NPC animation (0-20)
    std::vector<DialogueOption> options; ///< Response options
    std::vector<DialogueAction> actions; ///< Actions triggered by this dialogue
};

/**
 * @brief Dialogue manager
 * 
 * The DialogueManager handles NPC dialogue, conversation trees, and
 * associated actions for the Margin server.
 */
class DialogueManager {
public:
    /**
     * @brief Default constructor
     */
    DialogueManager();
    
    /**
     * @brief Destructor
     */
    ~DialogueManager();
    
    /**
     * @brief Initialize the dialogue manager
     * 
     * @return true if successful, false otherwise
     */
    bool Initialize();
    
    /**
     * @brief Shutdown the dialogue manager
     */
    void Shutdown();
    
    /**
     * @brief Load dialogue entries from database
     * 
     * @return true if successful, false otherwise
     */
    bool LoadDialogueEntries();
    
    /**
     * @brief Get a dialogue entry by ID
     * 
     * @param dialogueId Dialogue ID
     * @return Pointer to dialogue entry, or nullptr if not found
     */
    const DialogueEntry* GetDialogueEntry(uint32_t dialogueId) const;
    
    /**
     * @brief Get all dialogue entries
     * 
     * @return Map of dialogue ID to entry
     */
    const std::map<uint32_t, DialogueEntry>& GetAllDialogueEntries() const;
    
    /**
     * @brief Get initial dialogue for an NPC
     * 
     * @param npcId NPC ID
     * @return Dialogue ID, or 0 if not found
     */
    uint32_t GetInitialDialogue(uint32_t npcId);
    
    /**
     * @brief Get dialogue options for a player
     * 
     * @param playerId Player ID
     * @param dialogueId Dialogue ID
     * @return Vector of valid dialogue options
     */
    std::vector<DialogueOption> GetDialogueOptions(uint32_t playerId, uint32_t dialogueId);
    
    /**
     * @brief Select a dialogue option
     * 
     * @param playerId Player ID
     * @param dialogueId Current dialogue ID
     * @param optionId Selected option ID
     * @return Next dialogue ID, or 0 if conversation ends
     */
    uint32_t SelectDialogueOption(uint32_t playerId, uint32_t dialogueId, uint32_t optionId);
    
    /**
     * @brief Process dialogue actions
     * 
     * @param playerId Player ID
     * @param dialogueId Dialogue ID
     * @return true if actions were processed, false otherwise
     */
    bool ProcessDialogueActions(uint32_t playerId, uint32_t dialogueId);
    
    /**
     * @brief Check if a dialogue option is valid for a player
     * 
     * @param playerId Player ID
     * @param option Dialogue option to check
     * @return true if valid, false otherwise
     */
    bool IsDialogueOptionValid(uint32_t playerId, const DialogueOption& option);
    
    /**
     * @brief Create a dialogue message
     * 
     * @param dialogueId Dialogue ID
     * @param playerId Player ID
     * @return ByteBuffer containing the message
     */
    ByteBuffer CreateDialogueMessage(uint32_t dialogueId, uint32_t playerId);
    
    /**
     * @brief Get dialogue history for a player
     * 
     * @param playerId Player ID
     * @param npcId NPC ID
     * @return Vector of past dialogue IDs
     */
    std::vector<uint32_t> GetDialogueHistory(uint32_t playerId, uint32_t npcId);
    
    /**
     * @brief Add dialogue to player history
     * 
     * @param playerId Player ID
     * @param npcId NPC ID
     * @param dialogueId Dialogue ID
     */
    void AddDialogueToHistory(uint32_t playerId, uint32_t npcId, uint32_t dialogueId);
    
    /**
     * @brief Clear dialogue history for a player
     * 
     * @param playerId Player ID
     * @param npcId NPC ID (0 = all NPCs)
     */
    void ClearDialogueHistory(uint32_t playerId, uint32_t npcId = 0);
    
    /**
     * @brief Get statistics
     * 
     * @param dialogueCount Total dialogue interactions
     */
    void GetStats(uint32_t& dialogueCount);
    
    /**
     * @brief Update dialogue system
     * 
     * @param diff Time difference since last update in milliseconds
     */
    void Update(uint32_t diff);

private:
    /**
     * @brief Check player mission state
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @return Mission state (0=not started, 1=in progress, 2=completed)
     */
    uint8_t CheckPlayerMissionState(uint32_t playerId, uint32_t missionId);
    
    /**
     * @brief Check player skill level
     * 
     * @param playerId Player ID
     * @param skillId Skill ID
     * @return Skill level, or 0 if not found
     */
    uint8_t CheckPlayerSkillLevel(uint32_t playerId, uint32_t skillId);
    
    /**
     * @brief Execute dialogue action
     * 
     * @param playerId Player ID
     * @param action Dialogue action to execute
     * @return true if action was executed, false otherwise
     */
    bool ExecuteDialogueAction(uint32_t playerId, const DialogueAction& action);
    
    /**
     * @brief Load dialogue history from database
     * 
     * @param playerId Player ID
     * @return true if successful, false otherwise
     */
    bool LoadDialogueHistory(uint32_t playerId);
    
    /**
     * @brief Save dialogue history to database
     * 
     * @param playerId Player ID
     * @param npcId NPC ID
     * @return true if successful, false otherwise
     */
    bool SaveDialogueHistory(uint32_t playerId, uint32_t npcId);

    /**
     * @brief Dialogue entries map (ID to entry)
     */
    std::map<uint32_t, DialogueEntry> m_dialogueEntries;
    
    /**
     * @brief Initial dialogues map (NPC ID to dialogue ID)
     */
    std::map<uint32_t, uint32_t> m_initialDialogues;
    
    /**
     * @brief Dialogue history map (player ID + NPC ID to dialogue IDs)
     */
    std::map<std::pair<uint32_t, uint32_t>, std::vector<uint32_t>> m_dialogueHistory;
    
    /**
     * @brief Dialogue data mutex
     */
    std::mutex m_dialogueMutex;
    
    /**
     * @brief Dialogue interaction count
     */
    uint32_t m_dialogueCount;
    
    /**
     * @brief Manager is initialized flag
     */
    bool m_initialized;
};

#endif // _DIALOGUE_MANAGER_H_