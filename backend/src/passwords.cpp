#include <iostream>
#include "passwords.hpp"

namespace passwords
{
    std::unordered_set<std::string> generatePasswords(const int &numPasswords, const int &maxChars)
    {
        int minChars = 10;

        if (maxChars < minChars) {
            std::cout << "Password must have a minimum length of 10 characters\n";
            exit(0);
        }

        std::unordered_set<std::string> passwords;
        std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
        std::string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        std::string digits = "0123456789";
        std::string symbols = "!#$%&'()\\*+,-./:;<=>?@[]^_`{|}~\"";

        while (passwords.size() < numPasswords)
        {
            // randomize length between minChars and maxChars
            int length = minChars + rand() % ((maxChars + 1) - minChars);
            std::string password = ""; 
            bool hasLowercase = false, hasUppercase = false, hasDigit = false, hasSymbol = false;

            for (int i = 0; i < length; i++)
            {
                int charType = rand() % 4;
                int index;
                switch (charType)
                {
                case 0: // lowercase letter
                    hasLowercase = true;
                    if (!(hasLowercase && hasUppercase && hasDigit && hasSymbol)){
                        break;
                    }
                    index = rand() % lowercase.length();
                    password = password + lowercase[index];
                    break;
                case 1: // uppercase letter
                    hasUppercase = true;
                    if (!(hasLowercase && hasUppercase && hasDigit && hasSymbol)) {
                        break;
                    }
                    index = rand() % uppercase.length();
                    password = password + uppercase[index];
                    break;
                case 2: // digit
                    hasDigit = true;
                    if (!(hasLowercase && hasUppercase && hasDigit && hasSymbol)) {
                        break;
                    }
                    index = rand() % digits.length();
                    password = password + digits[index];
                    break;
                case 3: // symbol
                    hasSymbol = true;
                    if (!(hasLowercase && hasUppercase && hasDigit && hasSymbol)) {
                        break;
                    }
                    index = rand() % symbols.length();
                    password = password + symbols[index];
                    break;
                };
            };
            passwords.insert(password);
        };
        return passwords;
    };
};