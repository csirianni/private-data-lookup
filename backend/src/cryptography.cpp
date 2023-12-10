#include "cryptography.hpp"
#include "sodium.h"

namespace cryptography
{
    std::string hashAndEncryptPassword(const std::string &password, unsigned char *b, size_t offset)
    {
        // hash password to point
        unsigned char hash[crypto_core_ristretto255_HASHBYTES];
        crypto_generichash(hash, sizeof hash, (const unsigned char *)password.data(), password.length(), NULL, 0);
        unsigned char point[crypto_core_ristretto255_HASHBYTES];
        crypto_core_ristretto255_from_hash(point, hash);

        // encrypt password
        unsigned char encrypted_password[crypto_core_ristretto255_BYTES + offset];
        crypto_scalarmult_ristretto255(encrypted_password + offset, b, point);
        memcpy(encrypted_password, point, offset);

        return std::string(encrypted_password, encrypted_password + crypto_core_ristretto255_BYTES + offset);
    }

    std::string encryptPassword(const std::string &password, unsigned char *b, size_t offset)
    {
        std::string raw_password = password.substr(offset, crypto_core_ristretto255_BYTES);
        const unsigned char *data = (const unsigned char *)raw_password.data();
        unsigned char encrypted_password[crypto_core_ristretto255_BYTES];
        crypto_scalarmult_ristretto255(encrypted_password, b, data);
        return std::string(encrypted_password, encrypted_password + crypto_core_ristretto255_BYTES);
    }

    std::vector<std::string> encrypt(const std::unordered_set<std::string> &passwords, unsigned char *b, size_t offset)
    {
        std::vector<std::string> result;
        result.reserve(passwords.size());

        for (const auto &password : passwords)
        {
            result.push_back(hashAndEncryptPassword(password, b, offset));
        }

        return result;
    }
}