#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "LocationVector.h"
#include "ByteBuffer.h"
#include "MessageTypes.h"
#include <string>
#include <map>
#include <mutex>
#include <memory>

/**
 * @brief Base class for all game objects
 * 
 * GameObject is the base class for all interactive objects in the game world,
 * including players, NPCs, items, and environmental objects.
 */
class GameObject {
public:
    /**
     * @brief Constructor
     * 
     * @param objectId Unique object ID
     * @param objectType Type of object (see ObjectTypes enum)
     */
    GameObject(uint32_t objectId, uint16_t objectType);
    
    /**
     * @brief Virtual destructor
     */
    virtual ~GameObject();
    
    /**
     * @brief Get object ID
     * 
     * @return Object ID
     */
    uint32_t GetObjectId() const { return m_objectId; }
    
    /**
     * @brief Get object type
     * 
     * @return Object type (see ObjectTypes enum)
     */
    uint16_t GetObjectType() const { return m_objectType; }
    
    /**
     * @brief Get object position
     * 
     * @return Current position
     */
    const LocationVector& GetPosition() const { return m_position; }
    
    /**
     * @brief Set object position
     * 
     * @param position New position
     */
    void SetPosition(const LocationVector& position) { m_position = position; }
    
    /**
     * @brief Get object district
     * 
     * @return District ID
     */
    uint8_t GetDistrict() const { return m_district; }
    
    /**
     * @brief Set object district
     * 
     * @param district New district ID
     */
    void SetDistrict(uint8_t district) { m_district = district; }
    
    /**
     * @brief Get object name
     * 
     * @return Object name
     */
    const std::string& GetName() const { return m_name; }
    
    /**
     * @brief Set object name
     * 
     * @param name New name
     */
    void SetName(const std::string& name) { m_name = name; }
    
    /**
     * @brief Check if object is visible
     * 
     * @return true if visible, false otherwise
     */
    bool IsVisible() const { return m_isVisible; }
    
    /**
     * @brief Set object visibility
     * 
     * @param visible Visibility flag
     */
    void SetVisible(bool visible) { m_isVisible = visible; }
    
    /**
     * @brief Get object state flags
     * 
     * @return State flags
     */
    uint32_t GetStateFlags() const { return m_stateFlags; }
    
    /**
     * @brief Set object state flags
     * 
     * @param flags New state flags
     */
    void SetStateFlags(uint32_t flags) { m_stateFlags = flags; }
    
    /**
     * @brief Add state flag
     * 
     * @param flag Flag to add
     */
    void AddStateFlag(uint32_t flag) { m_stateFlags |= flag; }
    
    /**
     * @brief Remove state flag
     * 
     * @param flag Flag to remove
     */
    void RemoveStateFlag(uint32_t flag) { m_stateFlags &= ~flag; }
    
    /**
     * @brief Check if state flag is set
     * 
     * @param flag Flag to check
     * @return true if set, false otherwise
     */
    bool HasStateFlag(uint32_t flag) const { return (m_stateFlags & flag) != 0; }
    
    /**
     * @brief Get object scale
     * 
     * @return Scale factor
     */
    float GetScale() const { return m_scale; }
    
    /**
     * @brief Set object scale
     * 
     * @param scale New scale factor
     */
    void SetScale(float scale) { m_scale = scale; }
    
    /**
     * @brief Update the object
     * 
     * @param diff Time difference since last update in milliseconds
     */
    virtual void Update(uint32_t diff) = 0;
    
    /**
     * @brief Create an object creation message
     * 
     * @return Message for object creation
     */
    virtual std::shared_ptr<MessageBase> CreateObjectCreateMessage() const = 0;
    
    /**
     * @brief Create an object update message
     * 
     * @return Message for object update
     */
    virtual std::shared_ptr<MessageBase> CreateObjectUpdateMessage() const = 0;
    
    /**
     * @brief Create an object destroy message
     * 
     * @return Message for object destruction
     */
    virtual std::shared_ptr<MessageBase> CreateObjectDestroyMessage() const = 0;
    
    /**
     * @brief Serialize object data
     * 
     * @param data ByteBuffer to serialize to
     */
    virtual void Serialize(ByteBuffer& data) const = 0;
    
    /**
     * @brief Deserialize object data
     * 
     * @param data ByteBuffer to deserialize from
     * @return true if successful, false otherwise
     */
    virtual bool Deserialize(ByteBuffer& data) = 0;
    
    /**
     * @brief Handle interaction from another object
     * 
     * @param sourceObject Object initiating the interaction
     * @param interactionId Interaction type
     * @param data Additional interaction data
     * @return true if interaction was handled, false otherwise
     */
    virtual bool HandleInteraction(GameObject* sourceObject, uint16_t interactionId, ByteBuffer& data);
    
    /**
     * @brief Set a custom property
     * 
     * @param key Property key
     * @param value Property value
     */
    void SetProperty(const std::string& key, const std::string& value);
    
    /**
     * @brief Get a custom property
     * 
     * @param key Property key
     * @param defaultValue Default value if property not found
     * @return Property value, or defaultValue if not found
     */
    std::string GetProperty(const std::string& key, const std::string& defaultValue = "") const;
    
    /**
     * @brief Check if a custom property exists
     * 
     * @param key Property key
     * @return true if property exists, false otherwise
     */
    bool HasProperty(const std::string& key) const;
    
    /**
     * @brief Remove a custom property
     * 
     * @param key Property key
     * @return true if property was removed, false if not found
     */
    bool RemoveProperty(const std::string& key);
    
    /**
     * @brief Get all custom properties
     * 
     * @return Map of property keys to values
     */
    const std::map<std::string, std::string>& GetAllProperties() const;

protected:
    uint32_t m_objectId;                     ///< Unique object ID
    uint16_t m_objectType;                   ///< Object type
    LocationVector m_position;               ///< Current position
    uint8_t m_district;                      ///< Current district
    std::string m_name;                      ///< Object name
    bool m_isVisible;                        ///< Visibility flag
    uint32_t m_stateFlags;                   ///< State flags
    float m_scale;                           ///< Scale factor
    
    std::map<std::string, std::string> m_properties; ///< Custom properties
    mutable std::mutex m_propertiesMutex;    ///< Properties mutex
};

#endif // _GAME_OBJECT_H_