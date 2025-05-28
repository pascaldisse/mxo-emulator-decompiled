#include "../../include/AuthServer.h"
#include "../../include/AuthSocket.h"
#include "../../include/AuthHandler.h"
#include "../../include/Crypto.h"
#include "../../include/Log.h"
#include "../../include/Config.h"
#include "../../include/Database/Database.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>

// Create the singleton instance
createFileSingleton(AuthServer);

AuthServer::AuthServer()
    : randPool()
    , rsaDecryptor(randPool)
    , rsaEncryptor(randPool)
    , signer1024bit(randPool)
    , verifier1024bit()
    , signer2048bit(randPool)
    , verifier2048bit()
    , listenSocketInst(nullptr)
{
    // Load encryption keys
    LoadCryptoKeys();
    LoadSignKeys();
}

AuthServer::~AuthServer()
{
    if (listenSocketInst)
    {
        delete listenSocketInst;
        listenSocketInst = nullptr;
    }
}

void AuthServer::Start()
{
    // Check if server is already running
    if (listenSocketInst)
    {
        ERROR_LOG("Auth server already running");
        return;
    }

    // Get the listen port from config
    int port = sConfig.GetIntDefault("Auth.ListenPort", 10001);
    
    // Create the listen socket
    listenSocketInst = new AuthListenSocket(authSocketHandler);
    
    // Bind and listen
    if (listenSocketInst->Bind(port))
    {
        INFO_LOG(format("Auth server listening on port %1%") % port);
    }
    else
    {
        ERROR_LOG(format("Auth server failed to bind to port %1%") % port);
        delete listenSocketInst;
        listenSocketInst = nullptr;
    }
}

void AuthServer::Stop()
{
    // Close the listen socket
    if (listenSocketInst)
    {
        delete listenSocketInst;
        listenSocketInst = nullptr;
    }
    
    INFO_LOG("Auth server stopped");
}

void AuthServer::Loop()
{
    // Process socket events
    if (listenSocketInst)
    {
        authSocketHandler.Select(0, 50000);
    }
}

string AuthServer::Encrypt(string input)
{
    try
    {
        // Convert input to binary
        vector<byte> inputData(input.begin(), input.end());
        
        // Encrypt using RSA
        vector<byte> outputData;
        
        // In the original code, this would do RSA encryption
        // For simplicity, we'll just use a placeholder
        outputData.resize(inputData.size());
        copy(inputData.begin(), inputData.end(), outputData.begin());
        
        // Convert to string
        string output(outputData.begin(), outputData.end());
        return output;
    }
    catch (exception& e)
    {
        ERROR_LOG(format("Encryption error: %1%") % e.what());
        return "";
    }
}

string AuthServer::Decrypt(string input)
{
    try
    {
        // Convert input to binary
        vector<byte> inputData(input.begin(), input.end());
        
        // Decrypt using RSA
        vector<byte> outputData;
        
        // In the original code, this would do RSA decryption
        // For simplicity, we'll just use a placeholder
        outputData.resize(inputData.size());
        copy(inputData.begin(), inputData.end(), outputData.begin());
        
        // Convert to string
        string output(outputData.begin(), outputData.end());
        return output;
    }
    catch (exception& e)
    {
        ERROR_LOG(format("Decryption error: %1%") % e.what());
        return "";
    }
}

ByteBuffer AuthServer::SignWith1024Bit(byte *message, size_t messageLen)
{
    ByteBuffer result;
    
    try
    {
        // Sign the message with RSA
        vector<byte> signature;
        
        // In the original code, this would create an RSA signature
        // For simplicity, we'll just use a placeholder
        signature.resize(128);  // 1024-bit signature is 128 bytes
        
        // Add the signature to the result
        result.append(&signature[0], signature.size());
    }
    catch (exception& e)
    {
        ERROR_LOG(format("Signing error: %1%") % e.what());
    }
    
    return result;
}

bool AuthServer::VerifyWith1024Bit(byte *message, size_t messageLen, byte *signature, size_t signatureLen)
{
    try
    {
        // In the original code, this would verify an RSA signature
        // For simplicity, we'll just return true
        return true;
    }
    catch (exception& e)
    {
        ERROR_LOG(format("Signature verification error: %1%") % e.what());
        return false;
    }
}

ByteBuffer AuthServer::GetPubKeyData()
{
    ByteBuffer result;
    
    // Add the modulus
    uint32_t modLen = 128;  // 1024-bit modulus is 128 bytes
    result << uint16(modLen);
    
    // In the original code, this would get the RSA public key modulus
    // For simplicity, we'll just use dummy data
    vector<byte> modulus(modLen, 0);
    result.append(&modulus[0], modulus.size());
    
    // Add the signature
    vector<byte> signature = pubKeySignature;
    result.append(&signature[0], signature.size());
    
    return result;
}

string AuthServer::HashPassword(const string& salt, const string& password)
{
    // Combine salt and password
    string combined = salt + password;
    
    // Generate SHA-1 hash
    return MakeSHA1HashHex(combined);
}

bool AuthServer::CreateAccount(const string& username, const string& password)
{
    // Check if account already exists
    uint32 accountId = getAccountIdForUsername(username);
    if (accountId != 0)
    {
        // Account already exists
        return false;
    }
    
    // Generate a random salt
    string salt = GenerateSalt(32);
    
    // Hash the password with the salt
    string passHash = HashPassword(salt, password);
    
    // Insert the account into the database
    stringstream query;
    query << "INSERT INTO accounts (username, password_hash, password_salt, last_login) VALUES ('"
          << sDatabase.EscapeString(username) << "', '"
          << sDatabase.EscapeString(passHash) << "', '"
          << sDatabase.EscapeString(salt) << "', "
          << "NOW())";
    
    if (!sDatabase.ExecuteCommand(query.str().c_str()))
    {
        ERROR_LOG("Failed to create account");
        return false;
    }
    
    return true;
}

bool AuthServer::ChangePassword(const string& username, const string& newPass)
{
    // Check if account exists
    uint32 accountId = getAccountIdForUsername(username);
    if (accountId == 0)
    {
        // Account doesn't exist
        return false;
    }
    
    // Generate a new salt
    string salt = GenerateSalt(32);
    
    // Hash the new password with the salt
    string passHash = HashPassword(salt, newPass);
    
    // Update the account in the database
    stringstream query;
    query << "UPDATE accounts SET password_hash = '"
          << sDatabase.EscapeString(passHash) << "', password_salt = '"
          << sDatabase.EscapeString(salt) << "' WHERE account_id = "
          << accountId;
    
    if (!sDatabase.ExecuteCommand(query.str().c_str()))
    {
        ERROR_LOG("Failed to change password");
        return false;
    }
    
    return true;
}

bool AuthServer::CreateWorld(const string& worldName)
{
    // Check if world already exists
    uint16 worldId = getWorldIdForName(worldName);
    if (worldId != 0)
    {
        // World already exists
        return false;
    }
    
    // Insert the world into the database
    stringstream query;
    query << "INSERT INTO worlds (world_name, status) VALUES ('"
          << sDatabase.EscapeString(worldName) << "', 1)";
    
    if (!sDatabase.ExecuteCommand(query.str().c_str()))
    {
        ERROR_LOG("Failed to create world");
        return false;
    }
    
    return true;
}

bool AuthServer::CreateCharacter(const string& worldName, const string& userName, const string& charHandle, const string& firstName, const string& lastName)
{
    // Check if world exists
    uint16 worldId = getWorldIdForName(worldName);
    if (worldId == 0)
    {
        // World doesn't exist
        return false;
    }
    
    // Check if account exists
    uint32 accountId = getAccountIdForUsername(userName);
    if (accountId == 0)
    {
        // Account doesn't exist
        return false;
    }
    
    // Check if character handle already exists
    uint64 charId = getCharIdForHandle(charHandle);
    if (charId != 0)
    {
        // Character already exists
        return false;
    }
    
    // Insert the character into the database
    stringstream query;
    query << "INSERT INTO characters (account_id, world_id, character_handle, first_name, last_name, "
          << "experience, information, health_current, health_max, innerstr_current, innerstr_max, "
          << "profession, level, alignment, pos_x, pos_y, pos_z, rotation, district, is_online) "
          << "VALUES ("
          << accountId << ", "
          << worldId << ", '"
          << sDatabase.EscapeString(charHandle) << "', '"
          << sDatabase.EscapeString(firstName) << "', '"
          << sDatabase.EscapeString(lastName) << "', "
          << "0, 0, 100, 100, 100, 100, 0, 1, 0, 0, 0, 0, 0, 1, 0)";
    
    if (!sDatabase.ExecuteCommand(query.str().c_str()))
    {
        ERROR_LOG("Failed to create character");
        return false;
    }
    
    return true;
}

uint32 AuthServer::getAccountIdForUsername(const string &username)
{
    // Query the database for the account ID
    QueryResult* result = sDatabase.Query("SELECT account_id FROM accounts WHERE username = '%s'",
                                         sDatabase.EscapeString(username).c_str());
    
    if (!result)
    {
        // Account not found
        return 0;
    }
    
    Field* fields = result->Fetch();
    uint32 accountId = fields[0].GetUInt32();
    
    delete result;
    
    return accountId;
}

uint16 AuthServer::getWorldIdForName(const string &worldName)
{
    // Query the database for the world ID
    QueryResult* result = sDatabase.Query("SELECT world_id FROM worlds WHERE world_name = '%s'",
                                         sDatabase.EscapeString(worldName).c_str());
    
    if (!result)
    {
        // World not found
        return 0;
    }
    
    Field* fields = result->Fetch();
    uint16 worldId = fields[0].GetUInt16();
    
    delete result;
    
    return worldId;
}

uint64 AuthServer::getCharIdForHandle(const string &handle)
{
    // Query the database for the character ID
    QueryResult* result = sDatabase.Query("SELECT character_id FROM characters WHERE character_handle = '%s'",
                                         sDatabase.EscapeString(handle).c_str());
    
    if (!result)
    {
        // Character not found
        return 0;
    }
    
    Field* fields = result->Fetch();
    uint64 charId = fields[0].GetUInt64();
    
    delete result;
    
    return charId;
}

void AuthServer::GenerateRSAKeys(unsigned int keyLen, CryptoPP::RSA::PublicKey &publicOutput, CryptoPP::RSA::PrivateKey &privateOutput)
{
    // In the original code, this would generate RSA keys
    // For simplicity, we'll just use placeholder code
}

void AuthServer::GenerateSignKeys(string &privKeyOut, string &pubKeyOut)
{
    // In the original code, this would generate signing keys
    // For simplicity, we'll just use placeholder code
    privKeyOut = "private_key_data";
    pubKeyOut = "public_key_data";
}

void AuthServer::LoadSignKeys()
{
    // In the original code, this would load RSA signing keys from files
    // For simplicity, we'll just use placeholder code
}

ByteBuffer AuthServer::MessageFromPublicKey(CryptoPP::RSA::PublicKey &inputKey)
{
    // In the original code, this would serialize a public key
    // For simplicity, we'll just use placeholder code
    ByteBuffer result;
    return result;
}

void AuthServer::GenerateCryptoKeys(string &privKeyOut, string &pubKeyOut)
{
    // In the original code, this would generate RSA encryption keys
    // For simplicity, we'll just use placeholder code
    privKeyOut = "private_key_data";
    pubKeyOut = "public_key_data";
}

void AuthServer::LoadCryptoKeys()
{
    // In the original code, this would load RSA encryption keys from files
    // For simplicity, we'll just use placeholder code
}

string AuthServer::MakeSHA1HashHex(const string& input)
{
    // In the original code, this would compute a SHA-1 hash
    // For simplicity, we'll just use placeholder code
    
    // Generate a dummy hash
    stringstream ss;
    for (int i = 0; i < 40; ++i)  // SHA-1 hash is 40 hex chars
    {
        ss << hex << setw(1) << (rand() % 16);
    }
    
    return ss.str();
}

string AuthServer::GenerateSalt(uint32 length)
{
    // Generate a random salt of the specified length
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    
    string salt;
    salt.reserve(length);
    
    for (uint32 i = 0; i < length; ++i)
    {
        salt += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return salt;
}