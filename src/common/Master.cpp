#include "../../include/Master.h"
#include "../../include/Log.h"
#include "../../include/Config.h"
#include "../../include/Threading/Threading.h"
#include "../../include/Database/Database.h"
#include "../../include/AuthRunnable.h"
#include "../../include/MarginRunnable.h"
#include "../../include/GameRunnable.h"
#include "../../include/ConsoleThread.h"
#include "../../include/MersenneTwister.h"

#include <ctime>
#include <csignal>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep(x)
#else
#include <unistd.h>
#endif

// Create the singleton instance
createFileSingleton(Master);

volatile bool Master::m_stopEvent = false;

void Master::_OnSignal(int s)
{
    switch (s)
    {
        case SIGINT:
            Master::m_stopEvent = true;
            break;
        case SIGTERM:
            Master::m_stopEvent = true;
            break;
        case SIGABRT:
            Master::m_stopEvent = true;
            break;
#if defined(_WIN32)
        case SIGBREAK:
            Master::m_stopEvent = true;
            break;
#endif
    }

    signal(s, _OnSignal);
}

Master::Master()
{
}

Master::~Master()
{
}

#define REALITY_CONFIG "Reality.conf"

bool Master::Run()
{
    if (!sConfig.SetSource(REALITY_CONFIG))
    {
        CRITICAL_LOG(format("Could not find configuration file %1%.") % REALITY_CONFIG);
        exit(0);
    }

    INFO_LOG(format("Reality v0.01 Alpha %1% bit version started") % (sizeof(int*) * 8));

    if (!_StartDB())
    {
        CRITICAL_LOG("Error starting database!");

        Database::CleanupLibs();
        return false;
    }

    DEBUG_LOG("Initializing random number generators...");
    uint32_t seed = uint32_t(time(NULL));
    new MTRand(seed);
    srand(seed);

    _HookSignals();

    // Initialize thread manager
    ThreadPool.Startup();
    
    // Start server threads
    AuthRunnable *authRun = new AuthRunnable();
    ThreadPool.ExecuteTask(authRun);
    MarginRunnable *marginRun = new MarginRunnable();
    ThreadPool.ExecuteTask(marginRun);
    GameRunnable *gameRun = new GameRunnable();
    ThreadPool.ExecuteTask(gameRun);

    // Spawn console thread
    ConsoleThread *consoleRun = new ConsoleThread();
    ThreadPool.ExecuteTask(consoleRun);

    while (!Master::m_stopEvent)
    {
        sleep(100);
    }

    authRun->Terminate();
    marginRun->Terminate();
    gameRun->Terminate();

    consoleRun->Terminate();
    
    DEBUG_LOG("Exiting...");
    ThreadPool.ShowStats();

    _UnhookSignals();
    _StopDB();

    return 0;
}

// Database defines.
Database* Database_Main;

bool Master::_StartDB()
{
    Database_Main = Database::Create();

    std::string hostname, username, password, database;
    int port = 0;

    bool result = sConfig.GetString("Database.Username", &username);
    sConfig.GetString("Database.Password", &password);
    result = !result ? result : sConfig.GetString("Database.Hostname", &hostname);
    result = !result ? result : sConfig.GetString("Database.Name", &database);
    result = !result ? result : sConfig.GetInt("Database.Port", &port);

    if (result == false)
    {
        CRITICAL_LOG("sql: One or more parameters were missing from Database directive.");
        return false;
    }

    // Initialize it
    if (!sDatabase.Initialize(hostname.c_str(), (unsigned int)port, username.c_str(),
        password.c_str(), database.c_str(), sConfig.GetIntDefault("Database.ConnectionCount", 5),
        16384))
    {
        CRITICAL_LOG("sql: Main database initialization failed. Exiting.");
        return false;
    }

    return true;
}

void Master::_StopDB()
{
    sDatabase.EndThreads();
    sDatabase.Shutdown();
}

void Master::_HookSignals()
{
    signal(SIGINT, _OnSignal);
    signal(SIGTERM, _OnSignal);
    signal(SIGABRT, _OnSignal);
#if defined(_WIN32)
    signal(SIGBREAK, _OnSignal);
#endif
}

void Master::_UnhookSignals()
{
    signal(SIGINT, 0);
    signal(SIGTERM, 0);
    signal(SIGABRT, 0);
#if defined(_WIN32)
    signal(SIGBREAK, 0);
#endif
}

#if defined(_WIN32)
// Windows-specific crash handler implementation would go here
#endif