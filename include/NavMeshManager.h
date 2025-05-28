#ifndef _NAVMESH_MANAGER_H_
#define _NAVMESH_MANAGER_H_

#include "LocationVector.h"
#include <map>
#include <vector>
#include <string>
#include <mutex>
#include <memory>

// Forward declaration for Detour navmesh
class dtNavMesh;
class dtNavMeshQuery;
class dtQueryFilter;

/**
 * @brief Navigation mesh manager
 * 
 * The NavMeshManager handles pathfinding and spatial queries using
 * navigation meshes for each district.
 */
class NavMeshManager {
public:
    /**
     * @brief Default constructor
     */
    NavMeshManager();
    
    /**
     * @brief Destructor
     */
    ~NavMeshManager();
    
    /**
     * @brief Initialize the NavMesh manager
     * 
     * @return true if successful, false otherwise
     */
    bool Initialize();
    
    /**
     * @brief Shutdown the NavMesh manager
     */
    void Shutdown();
    
    /**
     * @brief Load a NavMesh for a district
     * 
     * @param districtId District ID
     * @param filename NavMesh file path
     * @return true if successful, false otherwise
     */
    bool LoadNavMesh(uint8_t districtId, const std::string& filename);
    
    /**
     * @brief Unload a NavMesh for a district
     * 
     * @param districtId District ID
     */
    void UnloadNavMesh(uint8_t districtId);
    
    /**
     * @brief Check if a NavMesh is loaded for a district
     * 
     * @param districtId District ID
     * @return true if loaded, false otherwise
     */
    bool IsNavMeshLoaded(uint8_t districtId) const;
    
    /**
     * @brief Find a path between two points
     * 
     * @param districtId District ID
     * @param start Start position
     * @param end End position
     * @return Vector of path points, empty if no path found
     */
    std::vector<LocationVector> FindPath(uint8_t districtId, const LocationVector& start, const LocationVector& end);
    
    /**
     * @brief Find a path between two points with maximum length
     * 
     * @param districtId District ID
     * @param start Start position
     * @param end End position
     * @param maxPathLength Maximum path length in world units
     * @return Vector of path points, empty if no path found or path too long
     */
    std::vector<LocationVector> FindPath(uint8_t districtId, const LocationVector& start, const LocationVector& end, float maxPathLength);
    
    /**
     * @brief Check if a position is valid (on the navmesh)
     * 
     * @param districtId District ID
     * @param position Position to check
     * @return true if valid, false otherwise
     */
    bool IsPositionValid(uint8_t districtId, const LocationVector& position);
    
    /**
     * @brief Get closest valid position
     * 
     * @param districtId District ID
     * @param position Position to check
     * @param maxDistance Maximum search distance
     * @return Valid position, or input position if none found
     */
    LocationVector GetClosestValidPosition(uint8_t districtId, const LocationVector& position, float maxDistance = 10.0f);
    
    /**
     * @brief Calculate distance between two points along navmesh
     * 
     * @param districtId District ID
     * @param start Start position
     * @param end End position
     * @return Distance in world units, -1 if no path found
     */
    float GetPathDistance(uint8_t districtId, const LocationVector& start, const LocationVector& end);
    
    /**
     * @brief Check line of sight between two points
     * 
     * @param districtId District ID
     * @param start Start position
     * @param end End position
     * @return true if line of sight exists, false otherwise
     */
    bool HasLineOfSight(uint8_t districtId, const LocationVector& start, const LocationVector& end);
    
    /**
     * @brief Get random point on navmesh
     * 
     * @param districtId District ID
     * @param center Center position
     * @param radius Search radius
     * @return Random point on navmesh, or center if none found
     */
    LocationVector GetRandomPoint(uint8_t districtId, const LocationVector& center, float radius);

private:
    /**
     * @brief NavMesh data structure
     */
    struct NavMeshData {
        dtNavMesh* navMesh;                ///< Detour navmesh
        dtNavMeshQuery* navQuery;          ///< Detour navmesh query
        dtQueryFilter* queryFilter;        ///< Query filter
        float cellSize;                    ///< Cell size
        float cellHeight;                  ///< Cell height
        float agentHeight;                 ///< Agent height
        float agentRadius;                 ///< Agent radius
        float agentMaxClimb;               ///< Agent max climb
        float agentMaxSlope;               ///< Agent max slope
    };
    
    /**
     * @brief Get NavMesh data for a district
     * 
     * @param districtId District ID
     * @return NavMeshData pointer, or nullptr if not found
     */
    NavMeshData* GetNavMeshData(uint8_t districtId);
    
    /**
     * @brief Get NavMesh data for a district (const version)
     * 
     * @param districtId District ID
     * @return NavMeshData pointer, or nullptr if not found
     */
    const NavMeshData* GetNavMeshData(uint8_t districtId) const;
    
    /**
     * @brief Simplify a path by removing unnecessary points
     * 
     * @param path Path to simplify
     * @return Simplified path
     */
    std::vector<LocationVector> SimplifyPath(const std::vector<LocationVector>& path);
    
    /**
     * @brief Find nearest point on navmesh
     * 
     * @param districtId District ID
     * @param position Position to find nearest point for
     * @param result Output result position
     * @param maxDistance Maximum search distance
     * @return true if found, false otherwise
     */
    bool FindNearestPoint(uint8_t districtId, const LocationVector& position, LocationVector& result, float maxDistance);

    /**
     * @brief NavMesh data map (district ID to NavMeshData)
     */
    std::map<uint8_t, NavMeshData*> m_navMeshes;
    
    /**
     * @brief NavMesh mutex
     */
    std::mutex m_navMeshMutex;
    
    /**
     * @brief Manager is initialized flag
     */
    bool m_initialized;
};

#endif // _NAVMESH_MANAGER_H_