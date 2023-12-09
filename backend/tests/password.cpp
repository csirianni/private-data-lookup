#define CATCH_CONFIG_MAIN
#include <regex>
#include <catch2/catch_test_macros.hpp>
#include "password.hpp"

namespace
{
    bool hasLettersAndDigit(const std::string &password)
    {
        const std::regex pattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d).+$");
        return regex_match(password, pattern);
    }

    bool hasSymbol(const std::string &password)
    {
        return password.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890") != std::string::npos;
    }
}

TEST_CASE("Test generatePassword creates valid passwords")
{
    std::unordered_set<std::string> passwords = password::generatePasswords(3, 12);
    // function generated 3 passwords
    CHECK(passwords.size() == 3);

    for (const auto &password : passwords)
    {
        CHECK(password.length() >= 10);
        CHECK(hasLettersAndDigit(password));
        CHECK(hasSymbol(password));
    }
}

TEST_CASE("Test generatePassword exception")
{
    CHECK_THROWS_AS(password::generatePasswords(3, 9), std::invalid_argument);
}