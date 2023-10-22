#define CATCH_CONFIG_MAIN
#include <regex>
#include <catch2/catch_all.hpp>
#include "../src/passwords.cpp"

std::unordered_set<std::string> passwordSet = passwords::generatePasswords(5, 24);

bool hasLettersAndDigit(std::string str)
{
    const std::regex pattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d).+$");
    return regex_match(str, pattern);
}

bool hasSymbol(std::string str)
{
    return str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos;
}

TEST_CASE("Test basic password requirements")
{
    // function generated 5 passwords
    REQUIRE(passwordSet.size() == 5);
    // a password has more than 10 characters
    auto password = *(passwordSet.begin());
    REQUIRE(password.length() >= 10);
    // a password meets all char requirements
    REQUIRE(hasLettersAndDigit(password));
    REQUIRE(hasSymbol(password));
}