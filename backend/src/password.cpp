#include <iostream>
#include "password.hpp"

namespace password
{
    std::unordered_set<std::string> generatePasswords(const int &numPasswords, const int &maxChars)
    {
        const int min_chars = 10;

        if (maxChars < min_chars) {
            std::cout << "Password must have a minimum length of 10 characters\n";
            exit(0);
        }

        std::unordered_set<std::string> passwords;
        const std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
        const std::string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const std::string digits = "0123456789";
        const std::string symbols = "!#$%&'()\\*+,-./:;<=>?@[]^_`{|}~\"";

        while (passwords.size() < numPasswords)
        {
            // randomize length between minChars and maxChars
            const size_t length = min_chars + rand() % ((maxChars + 1) - min_chars);
            std::string password = ""; 
            bool hasLowercase = false;
            bool hasUppercase = false;
            bool hasDigit = false;
            bool hasSymbol = false;

            while (password.length() < length)
            {
                int charType = rand() % 4;
                int index;
                switch (charType)
                {
                case 0: // lowercase letter
                    if (hasLowercase && !(hasUppercase && hasDigit && hasSymbol)){
                        break;
                    }
                    index = rand() % lowercase.length();
                    password = password + lowercase[index];
                    hasLowercase = true;
                    break;
                case 1: // uppercase letter
                    if (hasUppercase && !(hasLowercase && hasDigit && hasSymbol)) {
                        break;
                    }
                    index = rand() % uppercase.length();
                    password = password + uppercase[index];
                    hasUppercase = true;
                    break;
                case 2: // digit
                    if (hasDigit && !(hasLowercase && hasUppercase && hasSymbol)) {
                        break;
                    }
                    index = rand() % digits.length();
                    password = password + digits[index];
                    hasDigit = true;
                    break;
                case 3: // symbol
                    if (hasSymbol && !(hasLowercase && hasUppercase && hasDigit)) {
                        break;
                    }
                    index = rand() % symbols.length();
                    password = password + symbols[index];
                    hasSymbol = true;
                    break;
                };
            };
            passwords.insert(password);
        };
        return passwords;
    };
};