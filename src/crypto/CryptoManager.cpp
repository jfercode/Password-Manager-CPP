#include "CryptoManager.hpp"

// Default constructor
CryptoManager::CryptoManager()
{
    // Starts OpenSSL
    OpenSSL_add_all_algorithms();
}

CryptoManager::~CryptoManager() {}

// Generates random bytes
std::vector<unsigned char> CryptoManager::generateRandomBytes(size_t length) const
{
    std::vector<unsigned char> buffer(length); // buffer = bytes

    PrintLog(std::cout, CYAN "Crypto Manager" RESET " - Trying to generate random bytes..." RESET);
    if (RAND_bytes(buffer.data(), length) != 1)
        throw std::runtime_error("RAND_bytes failed");

    return buffer;
}

// Translate bytes into an hex string
std::string CryptoManager::bytesToHex(const std::vector<unsigned char> &bytes) const
{
    std::string hex;

    PrintLog(std::cout, CYAN "Crypto Manager" RESET " - translating bytes into hex...");
    for (unsigned char byte : bytes)
    {
        char buf[3];
        snprintf(buf, sizeof(buf), "%02x", byte);
        hex += buf;
    }
    return hex;
}

// Translate hex string into bytes
std::vector<unsigned char> CryptoManager::hexToBytes(const std::string &hex) const
{

    PrintLog(std::cout, CYAN "Crypto Manager" RESET " - translating hex into bytes...");

    // Validate input - hex string must have even length
    if (hex.length() % 2 != 0)
        throw std::runtime_error("FAILED TO translate hex string into bytes");

    // Prepare vector with reserved space
    std::vector<unsigned char> bytes;
    bytes.reserve(hex.length() / 2);

    // Convert each pair of hex characters to one byte
    for (size_t i = 0; i < hex.length(); i += 2)
    {

        // Extract 2 characters
        std::string byteStr = hex.substr(i, 2);
        try
        {
            // Convert from base 16
            unsigned char byte = static_cast<unsigned char>(std::stoi(byteStr, nullptr, 16));
            bytes.push_back(byte);
        }
        catch (const std::exception &e)
        {
            PrintLog(std::cerr, RED "Crypto Manager - Traslation hex to bytes error: %s" RESET, e.what());
            throw;
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
    PrintLog(std::cout, CYAN "Crypto Manager" RESET " - Hashing password...");

    // Generates random salt (16 bytes)
    auto salt_bytes = generateRandomBytes(16);

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
        hash);

    if (success != 1)
        throw std::runtime_error("PBKDF2 hashing failed");

    // Convert to hex
    std::vector<unsigned char> hash_vec(hash, hash + 32);
    std::string hash_hex = bytesToHex(hash_vec);
    std::string salt_hex = bytesToHex(salt_bytes);

    return {hash_hex, salt_hex};
}

// Verify that a password matches with it hash
bool CryptoManager::verifyPassword(
    const std::string &password,
    const std::string &storedHash,
    const std::string &salt,
    int iterations) const
{

    PrintLog(std::cout, CYAN "Crypto Manager" RESET " - Verifiying password...");

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
            computed_hash);
        if (success != 1)
            return false;

        // Compare with stored hash
        std::vector<unsigned char> computed_vec(computed_hash, computed_hash + 32);
        std::string computed_hex = bytesToHex(computed_vec);

        return computed_hex == storedHash;
    }
    catch (const std::exception &e)
    {
        PrintLog(std::cerr, RED "Crypto Manager - Verifying Password error: %s" RESET, e.what());
        return false;
    }
}

// Encrypt a password using AES-256-CBC cipher process
// Returns: {ciphertext_hex, iv_hex}
std::pair<std::string, std::string> CryptoManager::encryptPassword(const std::string &plaintext,
                                                                   const std::string &masterPassword,
                                                                   const std::string &salt) const
{
    PrintLog(std::cout, CYAN "Crypto Manager" RESET " - Encrypting password...");
    try
    {
        //  1. Generate random IV (16 bytes)
        // IV == Initialization vector
        auto iv_bytes = generateRandomBytes(16);

        //  2. Derivate key from Master Password
        // salt_hex to bytes conversion needed for the key derivation
        auto salt_bytes = hexToBytes(salt);

        // Derivate key with PBKDF2-SHA256
        unsigned char derived_key[32]; // 32 bytes => 256 bits

        int pbkdf2_res = PKCS5_PBKDF2_HMAC(
            masterPassword.c_str(),  // Password
            masterPassword.length(), // Password length
            salt_bytes.data(),       // salt bytes
            salt_bytes.size(),       // salt length
            10000,                   // iterations
            EVP_sha256(),            // hash algorithm
            32,                      // output key length
            derived_key              // output buffer
        );

        if (pbkdf2_res != 1)
            throw std::runtime_error("PBKDF2 derivation failed in encriptation process");

        PrintLog(std::cout, CYAN "Crypto Manager " RESET "- Key derived successfully");

        // 3. Cipher with AES-256-CBC algorithm
        // Create encryptation context with openssl
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        if (!ctx)
            throw std::runtime_error("Failed to create cipher context");

        // Start encryptation process
        int init_success = EVP_EncryptInit_ex(
            ctx,               // Context
            EVP_aes_256_cbc(), // Algorithm
            nullptr,           // Engine (use default with nullptr)
            derived_key,       // Derived key (32 bytes)
            iv_bytes.data()    // IV (16 bytes)
        );

        if (init_success != 1) // Check for result of init encryptation
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Encryptation init failed");
        }

        // Ciphertext bufer with maximun length of plaintext_len + 16 bits of memory block
        std::vector<unsigned char> ciphertext(plaintext.length() + 16);
        int ciphertext_len = 0;

        // Cipher the plaintext
        int update_success = EVP_EncryptUpdate(
            ctx,                                                        // Context
            ciphertext.data(),                                          // Output buffer
            &ciphertext_len,                                            // Output len
            reinterpret_cast<const unsigned char *>(plaintext.c_str()), // Plaintext (password without cipher)
            plaintext.length()                                          // Plaintext lenght
        );

        if (update_success != 1)
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Encryptation update failed");
        }

        // End encryptation
        int final_len = 0;
        int final_success = EVP_EncryptFinal_ex(
            ctx,                                // Context
            ciphertext.data() + ciphertext_len, // Buffer continuation
            &final_len                          // Final writed length
        );

        if (final_success != 1)
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Encryptation final failed");
        }

        ciphertext_len += final_len; // Update ciphertext total length
        EVP_CIPHER_CTX_free(ctx);    // Free context

        PrintLog(std::cout, CYAN "Crypto Manager" RESET " - Encryptation successful");

        //  4. Convert into hex
        // AES generates binaries bytes => convert its into hex to save in db
        std::vector<unsigned char> ciphertext_vec(ciphertext.begin(), ciphertext.begin() + ciphertext_len);

        std::string ciphertext_hex = bytesToHex(ciphertext_vec);
        std::string iv_hex = bytesToHex(iv_bytes);

        PrintLog(std::cout, CYAN "Crypto Manager" RESET " - Ciphertext and IV converted to hex");

        //  5. Return result (cipher hex password + iv in hex)
        return {ciphertext_hex, iv_hex};
    }
    catch (const std::exception &e)
    {
        PrintLog(std::cerr, RED "Crypto Manager - Encryption error: %s" RESET, e.what());
        throw;
    }
}

// Decrypt a cipher password returning the plaintext decrypted
std::string CryptoManager::decryptPassword(
    const std::string &ciphertext_hex,
    const std::string &iv_hex,
    const std::string &masterPassword,
    const std::string &salt) const
{
    PrintLog(std::cout, CYAN "Crypto Manager" RESET " - Decrypting password...");

    try
    {
        //  1. Convert hex values into bytes to operate with them
        auto ciphertext_bytes = hexToBytes(ciphertext_hex);
        auto iv_bytes = hexToBytes(iv_hex);

        PrintLog(std::cout, CYAN "Crypto Manager" RESET " - Hex converted to bytes");

        //  2. Derivate key from Master Password (it has to be the same ass cipher for decryptation process)
        // salt_hex to bytes conversion needed for the key derivation
        auto salt_bytes = hexToBytes(salt);

        // Derivate key with PBKDF2-SHA256
        unsigned char derived_key[32]; // 32 bytes => 256 bits

        int pbkdf2_res = PKCS5_PBKDF2_HMAC(
            masterPassword.c_str(),  // Password
            masterPassword.length(), // Password length
            salt_bytes.data(),       // salt bytes
            salt_bytes.size(),       // salt length
            10000,                   // iterations
            EVP_sha256(),            // hash algorithm
            32,                      // output key length
            derived_key              // output buffer
        );

        if (pbkdf2_res != 1)
            throw std::runtime_error("PBKDF2 derivation failed in desencryptation process");

        PrintLog(std::cout, CYAN "Crypto Manager " RESET "- Key derived successfully");

        //  3.  Decrypt with AES-256-CBC

        // Create context for decryption operation
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        if (!ctx)
            throw std::runtime_error("Failed to create cipher context");

        // Start decryption operation
        int init_success = EVP_DecryptInit_ex(
            ctx,
            EVP_aes_256_cbc(),
            nullptr,
            derived_key,
            iv_bytes.data());
        
        if (init_success != 1)
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Decryption init failed");
        }
        // Buffer for plaintext
        std::vector<unsigned char> plaintext(ciphertext_bytes.size() + 16);
        int plaintext_len = 0;

        // Decryptation
        int update_sucess = EVP_DecryptUpdate(
            ctx,
            plaintext.data(),
            &plaintext_len,
            ciphertext_bytes.data(),
            ciphertext_bytes.size());
        
        if (update_sucess != 1)
            throw std::runtime_error("Decryptation update failed");

        // Finish decryptation process (remove padding)
        int final_len = 0;
        int final_success = EVP_DecryptFinal_ex(
            ctx,
            plaintext.data() + plaintext_len,
            &final_len);
        
        if (final_success != 1)
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Decryptation final failed");
        }

        plaintext_len += final_len;
        EVP_CIPHER_CTX_free(ctx);

        PrintLog(std::cout, CYAN "Crypto Manager " RESET "- Decryption successful");

        //  4. Convert bytes to string
        std::string result(plaintext.begin(), plaintext.begin() + plaintext_len);

        // clean memory in plaintext
        OPENSSL_cleanse(plaintext.data(), plaintext.size());

        // returns password decrypted
        return result;
    }
    catch (const std::exception &e)
    {
        PrintLog(std::cerr, RED "Crypto Manager - Decryption error: %s" RESET, e.what());
        throw;
    }
}