#ifndef _MASTER_H_
#define _MASTER_H_

#include "Singleton.h"

/**
 * @brief Main server controller class
 * 
 * The Master class is the central controller for the MxoEmu server.
 * It manages the server lifecycle, initializes subsystems, and
 * coordinates between components.
 */
class Master : public Singleton<Master> {
public:
    /**
     * @brief Default constructor
     */
    Master();
    
    /**
     * @brief Destructor
     */
    ~Master();
    
    /**
     * @brief Run the server
     * 
     * Initialize all systems and enter the main loop.
     * 
     * @return 0 on success, error code on failure
     */
    bool Run();
    
    /**
     * @brief Signal handler callback
     * 
     * Processes operating system signals like SIGINT.
     * 
     * @param s The signal number
     */
    static void _OnSignal(int s);
    
    /**
     * @brief Stop flag
     * 
     * Set to true when the server should shut down.
     */
    static volatile bool m_stopEvent;

private:
    /**
     * @brief Initialize the database
     * 
     * @return true on success, false on failure
     */
    bool _StartDB();
    
    /**
     * @brief Shut down the database
     */
    void _StopDB();
    
    /**
     * @brief Set up signal handlers
     */
    void _HookSignals();
    
    /**
     * @brief Remove signal handlers
     */
    void _UnhookSignals();
};

#define sMaster Master::getSingleton()

#endif // _MASTER_H_