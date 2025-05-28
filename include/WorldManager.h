#ifndef _WORLD_MANAGER_H_
#define _WORLD_MANAGER_H_

#include "LocationVector.h"
#include "GameObject.h"
#include "NavMeshManager.h"
#include <map>
#include <vector>
#include <string>
#include <mutex>
#include <memory>

/**
 * @brief District data structure
 */
struct DistrictData {
    uint8_t id;                  ///< District ID
    std::string name;            ///< District name
    std::string description;     ///< District description
    std::string navMeshFile;     ///< NavMesh file for this district
    std::vector<uint32_t> adjacentDistricts; ///< Adjacent district IDs
    std::vector<LocationVector> hardlinePositions; ///< Hardline teleport positions
    std::vector<LocationVector> spawnPositions; ///< Player spawn positions
};

/**
 * @brief World manager
 * 
 * The WorldManager handles the game world state, objects, and spatial queries.
 */
class WorldManager {
public:
    /**
     * @brief Default constructor
     */
    WorldManager();
    
    /**
     * @brief Destructor
     */
    ~WorldManager();
    
    /**
     * @brief Initialize the world manager
     * 
     * @return true if successful, false otherwise
     */
    bool Initialize();
    
    /**
     * @brief Shutdown the world manager
     */
    void Shutdown();
    
    /**
     * @brief Load district data
     * 
     * @param districtId District ID to load
     * @return true if successful, false otherwise
     */
    bool LoadDistrict(uint8_t districtId);
    
    /**
     * @brief Get district data
     * 
     * @param districtId District ID to get
     * @return Pointer to district data, or nullptr if not found
     */
    const DistrictData* GetDistrictData(uint8_t districtId) const;
    
    /**
     * @brief Get all district data
     * 
     * @return Map of district ID to district data
     */
    const std::map<uint8_t, DistrictData>& GetAllDistricts() const;
    
    /**
     * @brief Add a game object to the world
     * 
     * @param object Game object to add
     * @return true if successful, false otherwise
     */
    bool AddObject(std::shared_ptr<GameObject> object);
    
    /**
     * @brief Remove a game object from the world
     * 
     * @param objectId Object ID to remove
     * @return true if successful, false otherwise
     */
    bool RemoveObject(uint32_t objectId);
    
    /**
     * @brief Get a game object by ID
     * 
     * @param objectId Object ID to find
     * @return Shared pointer to game object, or nullptr if not found
     */
    std::shared_ptr<GameObject> GetObject(uint32_t objectId);
    
    /**
     * @brief Get all objects in a district
     * 
     * @param districtId District ID to query
     * @return Vector of game objects in the district
     */
    std::vector<std::shared_ptr<GameObject>> GetObjectsInDistrict(uint8_t districtId);
    
    /**
     * @brief Get objects in range
     * 
     * @param position Center position
     * @param range Range in world units
     * @param districtId District ID (must match position's district)
     * @return Vector of game objects within range
     */
    std::vector<std::shared_ptr<GameObject>> GetObjectsInRange(const LocationVector& position, float range, uint8_t districtId);
    
    /**
     * @brief Get nearest object of a specific type
     * 
     * @param position Center position
     * @param objectType Object type to find
     * @param districtId District ID (must match position's district)
     * @param maxRange Maximum search range
     * @return Nearest object, or nullptr if none found
     */
    std::shared_ptr<GameObject> GetNearestObject(const LocationVector& position, uint16_t objectType, uint8_t districtId, float maxRange = 100.0f);
    
    /**
     * @brief Find path between two points
     * 
     * @param start Start position
     * @param end End position
     * @param districtId District ID
     * @return Vector of path points, empty if no path found
     */
    std::vector<LocationVector> FindPath(const LocationVector& start, const LocationVector& end, uint8_t districtId);
    
    /**
     * @brief Check if a position is valid (on the navmesh)
     * 
     * @param position Position to check
     * @param districtId District ID
     * @return true if valid, false otherwise
     */
    bool IsPositionValid(const LocationVector& position, uint8_t districtId);
    
    /**
     * @brief Get closest valid position
     * 
     * @param position Position to check
     * @param districtId District ID
     * @param maxDistance Maximum search distance
     * @return Valid position, or input position if none found
     */
    LocationVector GetClosestValidPosition(const LocationVector& position, uint8_t districtId, float maxDistance = 10.0f);
    
    /**
     * @brief Get random spawn position for a district
     * 
     * @param districtId District ID
     * @return Random spawn position
     */
    LocationVector GetRandomSpawnPosition(uint8_t districtId);
    
    /**
     * @brief Check line of sight between two points
     * 
     * @param start Start position
     * @param end End position
     * @param districtId District ID
     * @return true if line of sight exists, false otherwise
     */
    bool HasLineOfSight(const LocationVector& start, const LocationVector& end, uint8_t districtId);
    
    /**
     * @brief Update the world
     * 
     * @param diff Time difference since last update in milliseconds
     */
    void Update(uint32_t diff);

private:
    /**
     * @brief Load district data from database
     * 
     * @return true if successful, false otherwise
     */
    bool LoadDistrictData();
    
    /**
     * @brief Load world objects from database
     * 
     * @return true if successful, false otherwise
     */
    bool LoadWorldObjects();

    /**
     * @brief District data map
     */
    std::map<uint8_t, DistrictData> m_districts;
    
    /**
     * @brief Game object map (ID to object)
     */
    std::map<uint32_t, std::shared_ptr<GameObject>> m_objects;
    
    /**
     * @brief Object map mutex
     */
    std::mutex m_objectsMutex;
    
    /**
     * @brief District objects map (district ID to object IDs)
     */
    std::map<uint8_t, std::vector<uint32_t>> m_districtObjects;
    
    /**
     * @brief NavMesh manager
     */
    NavMeshManager m_navMeshManager;
    
    /**
     * @brief World is initialized flag
     */
    bool m_initialized;
};

#endif // _WORLD_MANAGER_H_