#ifndef _LOCATION_VECTOR_H_
#define _LOCATION_VECTOR_H_

#include <cmath>

/**
 * @brief Class for representing a 3D position and orientation
 * 
 * LocationVector represents a position (x, y, z) and orientation (o)
 * in the game world.
 */
class LocationVector {
public:
    /**
     * @brief Default constructor (origin, no rotation)
     */
    LocationVector() : x(0.0), y(0.0), z(0.0), o(0.0) {}
    
    /**
     * @brief Constructor with position
     */
    LocationVector(double _x, double _y, double _z) : x(_x), y(_y), z(_z), o(0.0) {}
    
    /**
     * @brief Constructor with position and orientation
     */
    LocationVector(double _x, double _y, double _z, double _o) : x(_x), y(_y), z(_z), o(_o) {}
    
    /**
     * @brief Copy constructor
     */
    LocationVector(const LocationVector& other) : x(other.x), y(other.y), z(other.z), o(other.o) {}
    
    /**
     * @brief Assignment operator
     */
    LocationVector& operator=(const LocationVector& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
            o = other.o;
        }
        return *this;
    }
    
    /**
     * @brief Calculate the distance to another point
     * 
     * @param other The other point
     * @return Distance in world units
     */
    double Distance(const LocationVector& other) const {
        double dx = other.x - x;
        double dy = other.y - y;
        double dz = other.z - z;
        return std::sqrt(dx*dx + dy*dy + dz*dz);
    }
    
    /**
     * @brief Calculate the 2D distance to another point (ignoring Z)
     * 
     * @param other The other point
     * @return Distance in world units
     */
    double Distance2D(const LocationVector& other) const {
        double dx = other.x - x;
        double dy = other.y - y;
        return std::sqrt(dx*dx + dy*dy);
    }
    
    /**
     * @brief Calculate the squared distance to another point
     * 
     * @param other The other point
     * @return Squared distance in world units
     */
    double DistanceSq(const LocationVector& other) const {
        double dx = other.x - x;
        double dy = other.y - y;
        double dz = other.z - z;
        return dx*dx + dy*dy + dz*dz;
    }
    
    /**
     * @brief Calculate the squared 2D distance to another point (ignoring Z)
     * 
     * @param other The other point
     * @return Squared distance in world units
     */
    double Distance2DSq(const LocationVector& other) const {
        double dx = other.x - x;
        double dy = other.y - y;
        return dx*dx + dy*dy;
    }
    
    /**
     * @brief Calculate the angle to another point
     * 
     * @param other The other point
     * @return Angle in radians
     */
    double AngleTo(const LocationVector& other) const {
        double dx = other.x - x;
        double dy = other.y - y;
        return std::atan2(dy, dx);
    }
    
    /**
     * @brief Addition operator
     */
    LocationVector operator+(const LocationVector& other) const {
        return LocationVector(x + other.x, y + other.y, z + other.z, o);
    }
    
    /**
     * @brief Subtraction operator
     */
    LocationVector operator-(const LocationVector& other) const {
        return LocationVector(x - other.x, y - other.y, z - other.z, o);
    }
    
    /**
     * @brief Equality operator
     */
    bool operator==(const LocationVector& other) const {
        return x == other.x && y == other.y && z == other.z && o == other.o;
    }
    
    /**
     * @brief Inequality operator
     */
    bool operator!=(const LocationVector& other) const {
        return !(*this == other);
    }
    
    /**
     * @brief Set the position components
     */
    void ChangePosition(double _x, double _y, double _z) {
        x = _x;
        y = _y;
        z = _z;
    }
    
    /**
     * @brief Set the orientation
     */
    void ChangeOrientation(double _o) {
        o = _o;
    }
    
    /**
     * @brief Move along a vector
     * 
     * @param dx X component of movement
     * @param dy Y component of movement
     * @param dz Z component of movement
     */
    void Move(double dx, double dy, double dz) {
        x += dx;
        y += dy;
        z += dz;
    }
    
    /**
     * @brief Move forward in the current orientation
     * 
     * @param distance Distance to move
     */
    void MoveForward(double distance) {
        x += distance * std::cos(o);
        y += distance * std::sin(o);
    }
    
    double x, y, z;  ///< Position coordinates
    double o;        ///< Orientation in radians
};

#endif // _LOCATION_VECTOR_H_