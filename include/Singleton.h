#ifndef _SINGLETON_H_
#define _SINGLETON_H_

/**
 * @brief Template class for implementing the Singleton pattern
 * 
 * This template provides a base implementation of the Singleton pattern,
 * ensuring that only one instance of a class exists and providing global
 * access to it.
 * 
 * @tparam T The class that will be a singleton
 */
template <class T>
class Singleton {
public:
    /**
     * @brief Get the singleton instance
     * 
     * @return Reference to the singleton instance
     */
    static T& getSingleton() {
        static T instance;
        return instance;
    }
    
    /**
     * @brief Get a pointer to the singleton instance
     * 
     * @return Pointer to the singleton instance
     */
    static T* getSingletonPtr() {
        return &getSingleton();
    }

protected:
    /**
     * @brief Default constructor
     * 
     * Protected to prevent direct instantiation
     */
    Singleton() {}
    
    /**
     * @brief Destructor
     */
    ~Singleton() {}
    
private:
    /**
     * @brief Copy constructor
     * 
     * Private to prevent copying
     */
    Singleton(const Singleton&) = delete;
    
    /**
     * @brief Assignment operator
     * 
     * Private to prevent assignment
     */
    Singleton& operator=(const Singleton&) = delete;
};

/**
 * @brief Macro to create a singleton implementation
 * 
 * This macro creates the static instance variable required by the
 * Singleton template.
 * 
 * @param TYPE The class type
 */
#define createFileSingleton(TYPE) template<> TYPE* Singleton<TYPE>::instance = nullptr

#endif // _SINGLETON_H_