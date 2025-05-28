#ifndef MXOSIM_AUTHSERVER_H
#define MXOSIM_AUTHSERVER_H

#include "Singleton.h"
#include "AuthHandler.h"
#include "AuthSocket.h"
#include "Crypto.h"
#include "ByteBuffer.h"

#include <Sockets/ListenSocket.h>
#include <string>
#include <vector>
#include <map>

/**
 * @brief Authentication server
 * 
 * The AuthServer manages user authentication, account creation,
 * character selection, and secure communication with clients.
 */
class AuthServer : public Singleton<AuthServer>
{
public:
    /**
     * @brief Default constructor
     */
    AuthServer();
    
    /**
     * @brief Destructor
     */
    ~AuthServer();
    
    /**
     * @brief Start the authentication server
     * 
     * Initializes and starts the server, binding to the configured port.
     */
    void Start();
    
    /**
     * @brief Stop the authentication server
     * 
     * Stops the server and releases resources.
     */
    void Stop();
    
    /**
     * @brief Process a server loop iteration
     * 
     * Handles pending socket events and processes authentication requests.
     */
    void Loop();
    
    /**
     * @brief Encrypt a string using RSA
     * 
     * @param input String to encrypt
     * @return Encrypted string
     */
    string Encrypt(string input);
    
    /**
     * @brief Decrypt a string using RSA
     * 
     * @param input String to decrypt
     * @return Decrypted string
     */
    string Decrypt(string input);
    
    /**
     * @brief Sign a message with 1024-bit RSA key
     * 
     * @param message Message to sign
     * @param messageLen Message length
     * @return Signature as a ByteBuffer
     */
    ByteBuffer SignWith1024Bit(byte *message, size_t messageLen);
    
    /**
     * @brief Verify a signature with 1024-bit RSA key
     * 
     * @param message Original message
     * @param messageLen Message length
     * @param signature Signature to verify
     * @param signatureLen Signature length
     * @return true if signature is valid, false otherwise
     */
    bool VerifyWith1024Bit(byte *message, size_t messageLen, byte *signature, size_t signatureLen);
    
    /**
     * @brief Get the public key data
     * 
     * @return Public key as a ByteBuffer
     */
    ByteBuffer GetPubKeyData();
    
    /**
     * @brief Hash a password with a salt
     * 
     * @param salt Salt value
     * @param password Password to hash
     * @return Hashed password
     */
    string HashPassword(const string& salt, const string& password);
    
    /**
     * @brief Create a new account
     * 
     * @param username Username
     * @param password Password
     * @return true if account was created, false otherwise
     */
    bool CreateAccount(const string& username, const string& password);
    
    /**
     * @brief Change an account's password
     * 
     * @param username Username
     * @param newPass New password
     * @return true if password was changed, false otherwise
     */
    bool ChangePassword(const string& username, const string& newPass);
    
    /**
     * @brief Create a new game world
     * 
     * @param worldName World name
     * @return true if world was created, false otherwise
     */
    bool CreateWorld(const string& worldName);
    
    /**
     * @brief Create a new character
     * 
     * @param worldName World name
     * @param userName Username
     * @param charHandle Character handle
     * @param firstName Character first name
     * @param lastName Character last name
     * @return true if character was created, false otherwise
     */
    bool CreateCharacter(const string& worldName, const string& userName, const string& charHandle, const string& firstName, const string& lastName);

private:
    /**
     * @brief Get account ID for a username
     * 
     * @param username Username to look up
     * @return Account ID, or 0 if not found
     */
    uint32 getAccountIdForUsername(const string &username);
    
    /**
     * @brief Get world ID for a world name
     * 
     * @param worldName World name to look up
     * @return World ID, or 0 if not found
     */
    uint16 getWorldIdForName(const string &worldName);
    
    /**
     * @brief Get character ID for a character handle
     * 
     * @param handle Character handle to look up
     * @return Character ID, or 0 if not found
     */
    uint64 getCharIdForHandle(const string &handle);
    
    /**
     * @brief Generate RSA key pair
     * 
     * @param keyLen Key length in bits
     * @param publicOutput Public key output
     * @param privateOutput Private key output
     */
    void GenerateRSAKeys(unsigned int keyLen, CryptoPP::RSA::PublicKey &publicOutput, CryptoPP::RSA::PrivateKey &privateOutput);
    
    /**
     * @brief Generate RSA signing keys
     * 
     * @param privKeyOut Private key output
     * @param pubKeyOut Public key output
     */
    void GenerateSignKeys(string &privKeyOut, string &pubKeyOut);
    
    /**
     * @brief Load RSA signing keys
     */
    void LoadSignKeys();
    
    /**
     * @brief Convert a public key to a message
     * 
     * @param inputKey Public key to convert
     * @return Key as a ByteBuffer
     */
    ByteBuffer MessageFromPublicKey(CryptoPP::RSA::PublicKey &inputKey);
    
    /**
     * @brief Generate RSA encryption keys
     * 
     * @param privKeyOut Private key output
     * @param pubKeyOut Public key output
     */
    void GenerateCryptoKeys(string &privKeyOut, string &pubKeyOut);
    
    /**
     * @brief Load RSA encryption keys
     */
    void LoadCryptoKeys();
    
    /**
     * @brief Create a SHA-1 hash of a string
     * 
     * @param input String to hash
     * @return Hexadecimal hash
     */
    string MakeSHA1HashHex(const string& input);
    
    /**
     * @brief Generate a random salt
     * 
     * @param length Salt length
     * @return Random salt string
     */
    string GenerateSalt(uint32 length);

    /**
     * @brief Authentication socket handler
     */
    AuthHandler authSocketHandler;
    
    /**
     * @brief Authentication listen socket
     */
    typedef ListenSocket<AuthSocket> AuthListenSocket;
    
    /**
     * @brief Listen socket instance
     */
    AuthListenSocket *listenSocketInst;

    /**
     * @brief Random number generator
     */
    CryptoPP::AutoSeededRandomPool randPool;

    /**
     * @brief RSA decryptor
     */
    CryptoPP::RSAES_OAEP_SHA_Decryptor rsaDecryptor;
    
    /**
     * @brief RSA encryptor
     */
    CryptoPP::RSAES_OAEP_SHA_Encryptor rsaEncryptor;

    /**
     * @brief RSA signer type
     */
    typedef CryptoPP::Weak::RSASSA_PKCS1v15_MD5_Signer RSASigner;
    
    /**
     * @brief RSA verifier type
     */
    typedef CryptoPP::Weak::RSASSA_PKCS1v15_MD5_Verifier RSAVerifier;
    
    /**
     * @brief 1024-bit RSA signer
     */
    RSASigner signer1024bit;
    
    /**
     * @brief 1024-bit RSA verifier
     */
    RSAVerifier verifier1024bit;
    
    /**
     * @brief 2048-bit RSA signer
     */
    RSASigner signer2048bit;
    
    /**
     * @brief 2048-bit RSA verifier
     */
    RSAVerifier verifier2048bit;

    /**
     * @brief Public key modulus
     */
    CryptoPP::Integer pubKeyModulus;
    
    /**
     * @brief Public key signature
     */
    vector<byte> pubKeySignature;
};

/**
 * @brief Global access to the authentication server singleton
 */
#define sAuth AuthServer::getSingleton()

#endif