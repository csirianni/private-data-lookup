#include <vector>
#include <string>
#include <unordered_set>

namespace cryptography
{
    /**
     * @brief Encrypt the provided set of passwords using secret key b.
     *
     * @param passwords the set of passwords to encrypt
     * @param b the secret key
     * @return std::vector<std::string> the encrypted passwords
     */
    std::vector<std::string> encrypt(const std::unordered_set<std::string> &passwords,
                                     unsigned char *b);

    /**
     * @brief Encrypt the provided point using secret key b.
     *
     * @param password the point to encrypt
     * @param b the secret key
     * @return std::string the encrypted password
     */
    std::string encryptPassword(const std::string &password, unsigned char *b);
}