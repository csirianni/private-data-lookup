#include <unordered_set>
#include <string>

#ifndef PASSWORDS_H
#define PASSWORDS_H

namespace password
{
    /**
     * @brief Returns a set of passwords, where each password contains at least one lowercase 
     * letter, at least one uppercase letter, at least one digit, and at least special character.
     * 
     * @param numPasswords The number of passwords to generate.
     * @param maxChars The maximum number of characters for a password.
     * @return std::unordered_set<std::string> 
     */
    std::unordered_set<std::string> generatePasswords(int num_passwords, int max_chars);
}
#endif // PASSWORDS_H