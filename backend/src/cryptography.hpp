#include <vector>
#include <string>
#include <cstring>
#include <unordered_set>

#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

namespace cryptography
{

    /**
     * @brief Hash and encrypt the provided password using secret key b.
     *
     * This function converts the provided password to a point on an elliptic curve and then encrypts it using the provided secret key.
     *
     * @param password the password to encrypt
     * @param b the secret key
     * @param offset the number of bytes to leak
     * @return std::string the encrypted password
     */
    std::string hashAndEncryptPassword(const std::string &password, unsigned char *b, size_t offset = 0);

    /**
     * @brief Encrypt the provided password using secret key b.
     *
     * This function does not hash the provided password before encrypting it.
     *
     * @param password the password to encrypt
     * @param b the secret key
     * @return std::string the encrypted password
     */
    std::string encryptPassword(const std::string &password, unsigned char *b, size_t offset = 0);

    /**
     * @brief Encrypt the provided set of passwords using secret key b.
     *
     * @param passwords the set of passwords to encrypt
     * @param b the secret key
     * @return std::vector<std::string> the encrypted passwords
     */
    std::vector<std::string> encrypt(const std::unordered_set<std::string> &passwords,
                                     unsigned char *b, size_t offset = 0);
}

#endif