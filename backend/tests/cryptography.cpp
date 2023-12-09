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
    unsigned char expectedHash[crypto_core_ristretto255_HASHBYTES];
    crypto_generichash(expectedHash, sizeof expectedHash, (const unsigned char *)password.data(), password.length(), NULL, 0);
    unsigned char expectedPoint[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_from_hash(expectedPoint, expectedHash);

    SECTION("without leaked byte")
    {
        for (int i = 0; i < 30; ++i)
        {
            // encrypt password
            std::string encryptedPasswordStr = cryptography::hashAndEncryptPassword(password, b);
            unsigned char encryptedPassword[crypto_core_ristretto255_BYTES];
            memcpy(encryptedPassword, encryptedPasswordStr.data(), crypto_core_ristretto255_BYTES);

            // unencrypt the password with the inverse of b
            unsigned char decryptedPassword[crypto_core_ristretto255_BYTES];
            int result = crypto_scalarmult_ristretto255(decryptedPassword, inverse, encryptedPassword);
            REQUIRE(result == 0);

            CHECK(std::memcmp(expectedPoint, decryptedPassword, crypto_core_ristretto255_BYTES) == 0);
        }
    }

    SECTION("with 1 leaked byte")
    {
        for (int i = 0; i < 30; ++i)
        {
            const size_t offset = 1;
            // encrypt password
            std::string encryptedPasswordStr = cryptography::hashAndEncryptPassword(password, b, offset);
            unsigned char encryptedPassword[crypto_core_ristretto255_BYTES + offset];
            memcpy(encryptedPassword, encryptedPasswordStr.data(), crypto_core_ristretto255_BYTES + offset);

            // decrypt password
            unsigned char decryptedPassword[crypto_core_ristretto255_BYTES + offset];
            int result = crypto_scalarmult_ristretto255(decryptedPassword + offset, inverse, encryptedPassword + offset);
            REQUIRE(result == 0);
            memcpy(decryptedPassword, encryptedPassword, offset);

            CHECK(std::memcmp(expectedPoint, decryptedPassword + offset, crypto_core_ristretto255_BYTES) == 0);
            CHECK(expectedPoint[0] == decryptedPassword[0]);
        }
    }
}