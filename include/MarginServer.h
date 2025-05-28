#ifndef _MARGIN_SERVER_H_
#define _MARGIN_SERVER_H_

#include "Singleton.h"
#include "MarginHandler.h"
#include "MarginSocket.h"
#include "ByteBuffer.h"
#include "MissionManager.h"
#include "DialogueManager.h"
#include "MessageTypes.h"

#include <Sockets/ListenSocket.h>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <memory>

/**
 * @brief Margin server
 * 
 * The MarginServer handles mission systems, narrative content, and
 * NPC dialogue in The Matrix Online.
 */
class MarginServer : public Singleton<MarginServer>
{
public:
    /**
     * @brief Default constructor
     */
    MarginServer();
    
    /**
     * @brief Destructor
     */
    ~MarginServer();
    
    /**
     * @brief Start the margin server
     * 
     * Initializes and starts the server, binding to the configured port.
     */
    void Start();
    
    /**
     * @brief Stop the margin server
     * 
     * Stops the server and releases resources.
     */
    void Stop();
    
    /**
     * @brief Process a server loop iteration
     * 
     * Handles pending socket events and processes margin requests.
     * 
     * @param diff Time difference since last update in milliseconds
     */
    void Loop(uint32_t diff);
    
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
     * @brief Update mission progress
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @param objectiveId Objective ID
     * @param progress Progress value
     * @return true if successful, false otherwise
     */
    bool UpdateMissionProgress(uint32_t playerId, uint32_t missionId, uint32_t objectiveId, uint32_t progress);
    
    /**
     * @brief Complete a mission
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @return true if successful, false otherwise
     */
    bool CompleteMission(uint32_t playerId, uint32_t missionId);
    
    /**
     * @brief Abandon a mission
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @return true if successful, false otherwise
     */
    bool AbandonMission(uint32_t playerId, uint32_t missionId);
    
    /**
     * @brief Get dialogue options for an NPC
     * 
     * @param playerId Player ID
     * @param npcId NPC ID
     * @return Vector of dialogue option IDs
     */
    std::vector<uint32_t> GetDialogueOptions(uint32_t playerId, uint32_t npcId);
    
    /**
     * @brief Select a dialogue option
     * 
     * @param playerId Player ID
     * @param npcId NPC ID
     * @param optionId Option ID
     * @return Response dialogue ID, or 0 if invalid
     */
    uint32_t SelectDialogueOption(uint32_t playerId, uint32_t npcId, uint32_t optionId);
    
    /**
     * @brief Get dialogue text
     * 
     * @param dialogueId Dialogue ID
     * @return Dialogue text
     */
    std::string GetDialogueText(uint32_t dialogueId);
    
    /**
     * @brief Get player's active missions
     * 
     * @param playerId Player ID
     * @return Map of mission ID to mission progress
     */
    std::map<uint32_t, uint32_t> GetActiveMissions(uint32_t playerId);
    
    /**
     * @brief Get player's completed missions
     * 
     * @param playerId Player ID
     * @return Vector of completed mission IDs
     */
    std::vector<uint32_t> GetCompletedMissions(uint32_t playerId);
    
    /**
     * @brief Check if player has completed a mission
     * 
     * @param playerId Player ID
     * @param missionId Mission ID
     * @return true if completed, false otherwise
     */
    bool HasCompletedMission(uint32_t playerId, uint32_t missionId);
    
    /**
     * @brief Get mission manager
     * 
     * @return Reference to mission manager
     */
    MissionManager& GetMissionManager();
    
    /**
     * @brief Get dialogue manager
     * 
     * @return Reference to dialogue manager
     */
    DialogueManager& GetDialogueManager();
    
    /**
     * @brief Get server statistics
     * 
     * @param activeMissions Total active missions
     * @param completedMissions Total completed missions
     * @param dialogueCount Dialogue interactions count
     * @param uptime Server uptime in seconds
     */
    void GetStats(uint32_t& activeMissions, uint32_t& completedMissions, uint32_t& dialogueCount, uint32_t& uptime);

private:
    /**
     * @brief Load mission data
     * 
     * @return true if successful, false otherwise
     */
    bool LoadMissionData();
    
    /**
     * @brief Load dialogue data
     * 
     * @return true if successful, false otherwise
     */
    bool LoadDialogueData();
    
    /**
     * @brief Update storyline events
     * 
     * @param diff Time difference since last update in milliseconds
     */
    void UpdateStorylineEvents(uint32_t diff);

    /**
     * @brief Margin socket handler
     */
    MarginHandler marginSocketHandler;
    
    /**
     * @brief Margin listen socket
     */
    typedef ListenSocket<MarginSocket> MarginListenSocket;
    
    /**
     * @brief Listen socket instance
     */
    MarginListenSocket *listenSocketInst;
    
    /**
     * @brief Mission manager
     */
    MissionManager m_missionManager;
    
    /**
     * @brief Dialogue manager
     */
    DialogueManager m_dialogueManager;
    
    /**
     * @brief Player missions map (player ID to mission IDs)
     */
    std::map<uint32_t, std::vector<uint32_t>> m_playerMissions;
    
    /**
     * @brief Player missions mutex
     */
    std::mutex m_missionsMutex;
    
    /**
     * @brief Server start time
     */
    uint32_t m_startTime;
    
    /**
     * @brief Last update time
     */
    uint32_t m_lastUpdate;
    
    /**
     * @brief Update interval in milliseconds
     */
    uint32_t m_updateInterval;
    
    /**
     * @brief Storyline event interval in milliseconds
     */
    uint32_t m_storylineEventInterval;
    
    /**
     * @brief Last storyline event time
     */
    uint32_t m_lastStorylineEvent;
    
    /**
     * @brief Dialogue interaction count
     */
    uint32_t m_dialogueCount;
};

/**
 * @brief Global access to the margin server singleton
 */
#define sMargin MarginServer::getSingleton()

#endif // _MARGIN_SERVER_H_