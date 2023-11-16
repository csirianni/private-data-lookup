#include "cryptography.hpp"
#include "sodium.h"

namespace cryptography
{

    std::vector<std::string> encrypt(const std::unordered_set<std::string> &passwords, unsigned char *b)
    {
        std::vector<std::string> result;
        result.reserve(passwords.size());

        for (const auto &password : passwords)
        {
            // 1. hash password to point
            unsigned char hash[crypto_generichash_BYTES];
            // TODO: check if this is correct
            crypto_generichash(hash, sizeof hash, (const unsigned char *)password.c_str(), password.length(), NULL, 0);

            // 2. multiply by b
            unsigned char encryptedPassword[crypto_core_ristretto255_BYTES];
            crypto_scalarmult_ristretto255(encryptedPassword, b, hash);

            std::string encryptedPasswordStr(reinterpret_cast<char*>(encryptedPassword));
            result.push_back(encryptedPasswordStr);
        }

        return result;
    }

    std::string encryptPassword(const std::string password, unsigned char *b) {
        // multiply by b
        unsigned char encryptedPassword[crypto_core_ristretto255_BYTES];
        crypto_scalarmult_ristretto255(encryptedPassword, b, (const unsigned char *)password.c_str());
        std::string encryptedPasswordStr(reinterpret_cast<char*>(encryptedPassword));
        return encryptedPasswordStr;
    }
}