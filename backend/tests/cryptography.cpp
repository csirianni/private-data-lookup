#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "cryptography.hpp"
#include "sodium.h"

TEST_CASE("Test hashAndEncryptPassword")
{
    // generate constants
    const std::string password = "TestPass1&";
    unsigned char b[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random(b);
    unsigned char inverse[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_invert(inverse, b);

    // compute expected value for password after computation
    unsigned char expected_hash[crypto_core_ristretto255_HASHBYTES];
    crypto_generichash(expected_hash, sizeof expected_hash, (const unsigned char *)password.data(), password.length(), NULL, 0);
    unsigned char expected_point[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_from_hash(expected_point, expected_hash);

    SECTION("without leaked byte")
    {
        for (int i = 0; i < 30; ++i)
        {
            // encrypt password
            std::string expected_password_str = cryptography::hashAndEncryptPassword(password, b);
            unsigned char encrypted_password[crypto_core_ristretto255_BYTES];
            memcpy(encrypted_password, expected_password_str.data(), crypto_core_ristretto255_BYTES);

            // unencrypt the password with the inverse of b
            unsigned char decrypted_password[crypto_core_ristretto255_BYTES];
            int result = crypto_scalarmult_ristretto255(decrypted_password, inverse, encrypted_password);
            REQUIRE(result == 0);

            CHECK(std::memcmp(expected_point, decrypted_password, crypto_core_ristretto255_BYTES) == 0);
        }
    }

    SECTION("with 1 leaked byte")
    {
        for (int i = 0; i < 30; ++i)
        {
            const size_t offset = 1;
            // encrypt password
            std::string encrypted_password_str = cryptography::hashAndEncryptPassword(password, b, offset);
            unsigned char encrypted_password[crypto_core_ristretto255_BYTES + offset];
            memcpy(encrypted_password, encrypted_password_str.data(), crypto_core_ristretto255_BYTES + offset);

            // decrypt password
            unsigned char decrypted_password[crypto_core_ristretto255_BYTES + offset];
            int result = crypto_scalarmult_ristretto255(decrypted_password + offset, inverse, encrypted_password + offset);
            REQUIRE(result == 0);
            memcpy(decrypted_password, encrypted_password, offset);

            CHECK(std::memcmp(expected_point, decrypted_password + offset, crypto_core_ristretto255_BYTES) == 0);
            CHECK(expected_point[0] == decrypted_password[0]);
        }
    }
}