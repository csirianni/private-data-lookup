#include "cryptography.hpp"
#include "sodium.h"

namespace cryptography
{

    std::string encryptPassword(const std::string &password, unsigned char *b)
    {
        // hash password to point
        unsigned char hash[crypto_core_ristretto255_HASHBYTES];
        crypto_generichash(hash, sizeof hash, (const unsigned char *)password.data(), password.length(), NULL, 0);
        unsigned char point[crypto_core_ristretto255_BYTES];
        crypto_core_ristretto255_from_hash(point, hash);

        // encrypt password
        unsigned char encryptedPassword[crypto_core_ristretto255_BYTES];
        crypto_scalarmult_ristretto255(encryptedPassword, b, point);
        return std::string(encryptedPassword, encryptedPassword + crypto_core_ristretto255_BYTES);
    }

    std::vector<std::string> encrypt(const std::unordered_set<std::string> &passwords, unsigned char *b)
    {
        std::vector<std::string> result;
        result.reserve(passwords.size());

        for (const auto &password : passwords)
        {
            result.push_back(encryptPassword(password, b));
        }

        return result;
    }
}