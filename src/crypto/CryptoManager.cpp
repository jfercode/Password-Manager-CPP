#include "CryptoManager.hpp"

// Default constructor
CryptoManager::CryptoManager()
{
    // Starts OpenSSL    
    OpenSSL_add_all_algorithms();
}

CryptoManager::~CryptoManager() {}

// Generates random bytes
std::vector<unsigned char> CryptoManager::generateRandomBytes(size_t length) const {
    
    std::vector<unsigned char> buffer(length);  // buffer = bytes
    
    PrintLog(std::cout, CYAN "Crypto Manager" RESET " - Trying to generate random bytes..." RESET);
    if (RAND_bytes(buffer.data(), length) != 1)
        throw std::runtime_error("RAND_bytes failed");
    
    return buffer;
}

// Translate bytes into an hex string
std::string CryptoManager::bytesToHex(const std::vector<unsigned char> &bytes) const {
    
    std::string hex;

    PrintLog(std::cout, CYAN "Crypto Manager" RESET" - translating bytes into hex...");
    for (unsigned char byte : bytes)
    {
        char buf[3];
        snprintf(buf, sizeof(buf), "%02x", byte);
        hex += buf;
    }
    return hex;
}

// Translate hex string into bytes
std::vector<unsigned char> CryptoManager::hexToBytes(const std::string &hex) const {
    
    PrintLog(std::cout, CYAN "Crypto Manager" RESET" - translating hex into bytes...");

    // Validate input - hex string must have even length
    if (hex.length() % 2 != 0)
        throw std::runtime_error("FAILED TO translate hex string into bytes");
    
    // Prepare vector with reserved space
    std::vector<unsigned char> bytes;
    bytes.reserve(hex.length() / 2);
    
    // Convert each pair of hex characters to one byte
    for (size_t i = 0; i < hex.length(); i += 2) {
        
        // Extract 2 characters
        std::string byteStr = hex.substr(i, 2);  
        try 
        {
            // Convert from base 16
            unsigned char byte = static_cast<unsigned char>(std::stoi(byteStr, nullptr, 16));
            bytes.push_back(byte);
        } 
        catch (...) {
            throw std::runtime_error("FAILED TO translate hex string into bytes");
        }
    }
    
    // Return the converted bytes
    return bytes;
}

// Password Hashing with PBKDF2
std::pair<std::string, std::string> CryptoManager::hashPassword(
    const std::string &password,
    int iterations) const
{

    PrintLog(std::cout, CYAN "Crypto Manager" RESET" - Hashing password...");

    // Generates random salt (16 bytes)
    auto    salt_bytes = generateRandomBytes(16);

    // Apply PBKDF2-SHA256
    unsigned char hash[32];

    int success = PKCS5_PBKDF2_HMAC(
        password.c_str(),
        password.length(),
        salt_bytes.data(),
        salt_bytes.size(),
        iterations,
        EVP_sha256(),
        32,
        hash
    );

    if (success != 1)
        throw std::runtime_error("PBKDF2 hashing failed");

    // Convert to hex
    std::vector<unsigned char> hash_vec(hash, hash + 32);
    std::string hash_hex = bytesToHex(hash_vec);
    std::string salt_hex = bytesToHex(salt_bytes);

    return {hash_hex, salt_hex};
}

// Verify that a password matches with it hash
bool    CryptoManager::verifyPassword(
    const std::string &password,
    const std::string &storedHash,
    const std::string &salt,
    int iterations) const
{
    
    PrintLog(std::cout, CYAN "Crypto Manager" RESET" - Verifiying password...");
    
    try
    {
        // Translate from salt (hex) to bytes
        auto salt_bytes = hexToBytes(salt);
        
        // Hash the password 
        unsigned char computed_hash[32];

        int success = PKCS5_PBKDF2_HMAC(
            password.c_str(),
            password.length(),
            salt_bytes.data(),
            salt_bytes.size(),
            iterations,
            EVP_sha256(),
            32,
            computed_hash
        );
        if (success != 1)
            return false;

        // Compare with stored hash
        std::vector<unsigned char> computed_vec(computed_hash, computed_hash +  32);
        std::string computed_hex = bytesToHex(computed_vec);

        return computed_hex == storedHash;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

// // Derive an encryption key from a password
// std::string CryptoManager::deriveKey(
//     const std::string &password,
//     const std::string &salt,
//     int iterations) {}
