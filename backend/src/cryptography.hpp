#include <vector>
#include <string>
#include <unordered_set>

namespace cryptography
{

    /**
     * @brief Encrypt the provided point using secret key b.
     *
     * This function converts the provided password to a point on an elliptic curve and then encrypts it using the provided secret key.
     *
     * @param password the password to encrypt
     * @param b the secret key
     * @return std::string the encrypted password
     */

    std::string encryptPassword(const std::string &password, unsigned char *b);
    /**
     * @brief Encrypt the provided set of passwords using secret key b.
     *
     * @param passwords the set of passwords to encrypt
     * @param b the secret key
     * @return std::vector<std::string> the encrypted passwords
     */
    std::vector<std::string> encrypt(const std::unordered_set<std::string> &passwords,
                                     unsigned char *b);
}