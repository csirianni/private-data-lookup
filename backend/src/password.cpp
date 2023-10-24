#include <iostream>
#include "password.hpp"

namespace password
{
    std::unordered_set<std::string> generatePasswords(int num_passwords, int max_chars)
    {
        const int min_chars = 10;

        // ensure max_chars is greater than min_chars
        try
        {
            if (max_chars < min_chars)
            {
                throw(max_chars);
            }
        }
        catch (int max_chars)
        {
            std::cout << "Password must have a minimum length of 10 characters\n";
            std::cout << "You inputted a maximum length of " << max_chars << " characters \n";
        }

        std::unordered_set<std::string> passwords;
        const std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
        const std::string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const std::string digits = "0123456789";
        const std::string symbols = "!#$%&()*+,-./:;<=>?@[]^_`{|}~";

        while (passwords.size() < num_passwords)
        {
            // randomize length between min_chars and max_chars
            const size_t length = min_chars + rand() % ((max_chars + 1) - min_chars);
            std::string password = "";
            bool has_lowercase = false;
            bool has_uppercase = false;
            bool has_digit = false;
            bool has_symbol = false;

            while (password.length() < length)
            {
                int char_type = rand() % 4;
                int index;
                switch (char_type)
                {
                case 0: // lowercase letter
                    if (has_lowercase && !(has_uppercase && has_digit && has_symbol))
                    {
                        break;
                    }
                    index = rand() % lowercase.length();
                    password = password + lowercase[index];
                    has_lowercase = true;
                    break;
                case 1: // uppercase letter
                    if (has_uppercase && !(has_lowercase && has_digit && has_symbol))
                    {
                        break;
                    }
                    index = rand() % uppercase.length();
                    password = password + uppercase[index];
                    has_uppercase = true;
                    break;
                case 2: // digit
                    if (has_digit && !(has_lowercase && has_uppercase && has_symbol))
                    {
                        break;
                    }
                    index = rand() % digits.length();
                    password = password + digits[index];
                    has_digit = true;
                    break;
                case 3: // symbol
                    if (has_symbol && !(has_lowercase && has_uppercase && has_digit))
                    {
                        break;
                    }
                    index = rand() % symbols.length();
                    password = password + symbols[index];
                    has_symbol = true;
                    break;
                };
            };
            passwords.insert(password);
        };
        return passwords;
    };
};