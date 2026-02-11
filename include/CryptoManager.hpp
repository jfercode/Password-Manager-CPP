#ifndef CRYPTOMANAGER_HPP
# define CRYPTOMANAGER_HPP

#include "library.hpp"
#include <openssl/rand.h>	// random generation
#include <openssl/evp.h>	// PBKDF2
#include <openssl/err.h>	// error handling

class CryptoManager
{
    private:
		// Generates random bytes
		std::vector<unsigned char> generateRandomBytes(size_t length) const;

		// Translate bytes into an hex string
		std::string	bytesToHex(const std::vector<unsigned char>& bytes) const;

		// Translate hex string into bytes
		std::vector<unsigned char>	hexToBytes(const std::string& hex) const;

    public:
        CryptoManager();
        ~CryptoManager();

		// Password Hashing with PBKDF2
        std::pair<std::string, std::string> hashPassword(
            const std::string &password,
            int iterations = 10000
		) const;
        
		// Verify that a password matches with it hash
        bool verifyPassword(
            const std::string &password, 
            const std::string &storedHash,
            const std::string &salt,
            int iterations = 10000
		) const;

		// std::string	deriveKey(
		// 	const std::string &password,
		// 	const std::string &salt,
		// 	int	iterations = 10000
		// );
};

#endif