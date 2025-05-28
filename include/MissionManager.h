#ifndef _MISSION_MANAGER_H_
#define _MISSION_MANAGER_H_

#include "ByteBuffer.h"
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <memory>

/**
 * @brief Mission objective structure
 */
struct MissionObjective {
    uint32_t id;                    ///< Objective ID
    std::string description;        ///< Objective description
    uint32_t targetValue;           ///< Target value to complete
    bool isOptional;                ///< Is this objective optional
    std::string completionText;     ///< Text shown on completion
    uint32_t rewardExperience;      ///< Experience reward
    uint32_t rewardInformation;     ///< Information (currency) reward
    std::vector<uint32_t> rewardItems; ///< Item rewards
};

/**
 * @brief Mission prerequisite structure
 */
struct MissionPrerequisite {
    enum PrerequisiteType {
        LEVEL = 0,
        FACTION = 1,
        PROFESSION = 2,
        COMPLETED_MISSION = 3,
        ITEM_POSSESSION = 4,
        SKILL_LEVEL = 5,
        DISTRICT_ACCESS = 6
    };
    
    PrerequisiteType type;          ///< Prerequisite type
    uint32_t value;                 ///< Required value
};

/**
 * @brief Mission definition structure
 */
struct MissionDefinition {
    uint32_t id;                    ///< Mission ID
    std::string name;               ///< Mission name
    std::string description;        ///< Mission description
    uint8_t minLevel;               ///< Minimum player level
    uint8_t maxLevel;               ///< Maximum player level
    uint8_t faction;                ///< Required faction (0=any)
    uint32_t giverNpcId;            ///< NPC that gives the mission
    uint32_t turnInNpcId;           ///< NPC that receives mission completion
    bool repeatable;                ///< Can this mission be repeated
    uint32_t cooldownTime;          ///< Cooldown before repeating (seconds)
    std::string startDialogue;      ///< Initial dialogue text
    std::string completionDialogue; ///< Completion dialogue text
    std::string failureDialogue;    ///< Failure dialogue text
    std::vector<MissionObjective> objectives; ///< Mission objectives
    std::vector<MissionPrerequisite> prerequisites; ///< Mission prerequisites
};

/**
 * @brief Mission instance structure
 */
struct MissionInstance {
    uint32_t missionId;             ///< Mission ID
    uint32_t playerId;              ///< Player ID
    uint32_t startTime;             ///< Time when mission was started
    bool completed;                 ///< Is mission completed
    bool failed;                    ///< Is mission failed
    std::map<uint32_t, uint32_t> objectiveProgress; ///< Objective progress (ID to value)
};

/**
 * @brief Mission manager
 * 
 * The MissionManager handles mission definitions, instances, and progression
 * for the Margin server.
 */
class MissionManager {
public:
    /**
     * @brief Default constructor
     */
    MissionManager();
    
    /**
     * @brief Destructor
     */
    ~MissionManager();
    
    /**
     * @brief Initialize the mission manager
     * 
     * @return true if successful, false otherwise
     */
    bool Initialize();
    
    /**
     * @brief Shutdown the mission manager
     */
    void Shutdown();
    
    /**
     * @brief Load mission definitions from database
     * 
     * @return true if successful, false otherwise
     */
    bool LoadMissionDefinitions();
    
    /**
     * @brief Get a mission definition by ID
     * 
     * @param missionId Mission ID
     * @return Pointer to mission definition, or nullptr if not found
     */
    const MissionDefinition* GetMissionDefinition(uint32_t missionId) const;
    
    /**
     * @brief Get all mission definitions
     * 
     * @return Map of mission ID to definition
     */
    const std::map<uint32_t, MissionDefinition>& GetAllMissionDefinitions() const;
    
    /**
     * @brief Get available missions for a player
     * 
     * @param playerId Player ID
     * @param professionId Player profession ID
     * @param level Player level
     * @param alignment Player alignment
     * @return Vector of available mission IDs
     */
    std::vector<uint32_t> GetAvailableMissions(uint32_t playerId, uint32_t professionId, uint8_t level, uint8_t alignment);
    
    /**
     * @brief Start a mission for a player
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @return true if successful, false otherwise
     */
    bool StartMission(uint32_t playerId, uint32_t missionId);
    
    /**
     * @brief Update mission objective progress
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @param objectiveId Objective ID
     * @param progress Progress value
     * @return true if successful, false otherwise
     */
    bool UpdateObjectiveProgress(uint32_t playerId, uint32_t missionId, uint32_t objectiveId, uint32_t progress);
    
    /**
     * @brief Complete a mission
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @return true if successful, false otherwise
     */
    bool CompleteMission(uint32_t playerId, uint32_t missionId);
    
    /**
     * @brief Fail a mission
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @return true if successful, false otherwise
     */
    bool FailMission(uint32_t playerId, uint32_t missionId);
    
    /**
     * @brief Abandon a mission
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @return true if successful, false otherwise
     */
    bool AbandonMission(uint32_t playerId, uint32_t missionId);
    
    /**
     * @brief Get active missions for a player
     * 
     * @param playerId Player ID
     * @return Vector of mission instances
     */
    std::vector<MissionInstance> GetActiveMissions(uint32_t playerId);
    
    /**
     * @brief Get a specific mission instance
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @return Pointer to mission instance, or nullptr if not found
     */
    const MissionInstance* GetMissionInstance(uint32_t playerId, uint32_t missionId);
    
    /**
     * @brief Check if a player has completed a mission
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @return true if completed, false otherwise
     */
    bool HasCompletedMission(uint32_t playerId, uint32_t missionId);
    
    /**
     * @brief Get completed missions for a player
     * 
     * @param playerId Player ID
     * @return Vector of completed mission IDs
     */
    std::vector<uint32_t> GetCompletedMissions(uint32_t playerId);
    
    /**
     * @brief Check if a mission objective is complete
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @param objectiveId Objective ID
     * @return true if complete, false otherwise
     */
    bool IsObjectiveComplete(uint32_t playerId, uint32_t missionId, uint32_t objectiveId);
    
    /**
     * @brief Get objective progress
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @param objectiveId Objective ID
     * @return Progress value, or 0 if not found
     */
    uint32_t GetObjectiveProgress(uint32_t playerId, uint32_t missionId, uint32_t objectiveId);
    
    /**
     * @brief Check if all mission objectives are complete
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @return true if all objectives are complete, false otherwise
     */
    bool AreAllObjectivesComplete(uint32_t playerId, uint32_t missionId);
    
    /**
     * @brief Create a mission progress message
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @return ByteBuffer containing the message
     */
    ByteBuffer CreateMissionProgressMessage(uint32_t playerId, uint32_t missionId);
    
    /**
     * @brief Create a mission completion message
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @return ByteBuffer containing the message
     */
    ByteBuffer CreateMissionCompletionMessage(uint32_t playerId, uint32_t missionId);
    
    /**
     * @brief Create a mission list message
     * 
     * @param playerId Player ID
     * @return ByteBuffer containing the message
     */
    ByteBuffer CreateMissionListMessage(uint32_t playerId);
    
    /**
     * @brief Get statistics
     * 
     * @param activeMissions Total active missions
     * @param completedMissions Total completed missions
     */
    void GetStats(uint32_t& activeMissions, uint32_t& completedMissions);
    
    /**
     * @brief Update mission instances
     * 
     * @param diff Time difference since last update in milliseconds
     */
    void Update(uint32_t diff);

private:
    /**
     * @brief Check if a player meets the prerequisites for a mission
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @param professionId Player profession ID
     * @param level Player level
     * @param alignment Player alignment
     * @return true if prerequisites are met, false otherwise
     */
    bool CheckPrerequisites(uint32_t playerId, uint32_t missionId, uint32_t professionId, uint8_t level, uint8_t alignment);
    
    /**
     * @brief Load player mission data from database
     * 
     * @param playerId Player ID
     * @return true if successful, false otherwise
     */
    bool LoadPlayerMissionData(uint32_t playerId);
    
    /**
     * @brief Save mission instance to database
     * 
     * @param instance Mission instance to save
     * @return true if successful, false otherwise
     */
    bool SaveMissionInstance(const MissionInstance& instance);
    
    /**
     * @brief Delete mission instance from database
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @return true if successful, false otherwise
     */
    bool DeleteMissionInstance(uint32_t playerId, uint32_t missionId);
    
    /**
     * @brief Add completed mission to database
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @return true if successful, false otherwise
     */
    bool AddCompletedMission(uint32_t playerId, uint32_t missionId);
    
    /**
     * @brief Load completed missions from database
     * 
     * @param playerId Player ID
     * @return true if successful, false otherwise
     */
    bool LoadCompletedMissions(uint32_t playerId);

    /**
     * @brief Mission definitions map (ID to definition)
     */
    std::map<uint32_t, MissionDefinition> m_missionDefinitions;
    
    /**
     * @brief Mission instances map (player ID + mission ID to instance)
     */
    std::map<std::pair<uint32_t, uint32_t>, MissionInstance> m_missionInstances;
    
    /**
     * @brief Completed missions map (player ID to mission IDs)
     */
    std::map<uint32_t, std::vector<uint32_t>> m_completedMissions;
    
    /**
     * @brief Mission data mutex
     */
    std::mutex m_missionMutex;
    
    /**
     * @brief Manager is initialized flag
     */
    bool m_initialized;
};

#endif // _MISSION_MANAGER_H_