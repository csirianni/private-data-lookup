#include <unordered_set>
#include <string>

#ifndef PASSWORDS_H
#define PASSWORDS_H

namespace password
{
    /**
     * @brief Generate a set of passwords where each password contains at least one lowercase
     * letter, at least one uppercase letter, at least one digit, and at least special character.
     *
     * @param num_passwords The number of passwords to generate.
     * @param max_chars The maximum number of characters for a password.
     * @return std::unordered_set<std::string> The passwords generated.
     * @throws std::invalid_argument If max_chars is < min_chars (locally defined).
     */
    std::unordered_set<std::string> generatePasswords(int num_passwords, int max_chars);
}
#endif // PASSWORDS_H