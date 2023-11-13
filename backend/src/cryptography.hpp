#include <vector>
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

}