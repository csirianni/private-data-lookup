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
     *
     * @warning The password must be hashed to a point before being passed to this function.
     * @warning The result of this function is not a string, but a vector of bytes (excluding the null terminator).
     */
    std::string encryptPoint(const std::string &password, unsigned char *b);
}