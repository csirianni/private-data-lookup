#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "cryptography.hpp"
#include "sodium.h"

TEST_CASE("Test encryption")
{
    unsigned char b[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random(b);
    unsigned char inverse[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_invert(inverse, b);

    std::string password = "TestPass1&";
    unsigned char hash[crypto_core_ristretto255_HASHBYTES];
    // TODO: Using parenthetical casting is dangerous
    crypto_generichash(hash, sizeof hash, (const unsigned char *)password.data(), password.length(), NULL, 0);
    unsigned char point[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_from_hash(point, hash);

    // multiply by b
    unsigned char encryptedPassword[crypto_core_ristretto255_BYTES];
    crypto_scalarmult_ristretto255(encryptedPassword, b, point);
    std::string ePassword = std::string(encryptedPassword, encryptedPassword + crypto_core_ristretto255_BYTES);
    // TODO: use c_str
    const unsigned char *newPassword = (const unsigned char *)ePassword.data();
    // unencrypt the password with the inverse of b
    unsigned char decryptedPassword[crypto_core_ristretto255_BYTES];
    crypto_scalarmult_ristretto255(decryptedPassword, inverse, newPassword);
    CHECK(std::memcmp(decryptedPassword, point, crypto_core_ristretto255_BYTES) == 0);
}

TEST_CASE("Test encryptPassword")
{
    for (int i = 0; i < 30; ++i)
    {
        // generate constants
        unsigned char b[crypto_core_ristretto255_SCALARBYTES];
        crypto_core_ristretto255_scalar_random(b);
        unsigned char inverse[crypto_core_ristretto255_SCALARBYTES];
        crypto_core_ristretto255_scalar_invert(inverse, b);
        const std::string password = "TestPass1&";

        // encrypt password
        std::string encryptedPasswordStr = cryptography::encryptPoint(password, b);
        unsigned char encryptedPassword[crypto_core_ristretto255_BYTES];
        std::memcpy(encryptedPassword, encryptedPasswordStr.data(), crypto_core_ristretto255_BYTES);

        // unencrypt the password with the inverse of b
        unsigned char decryptedPassword[crypto_core_ristretto255_BYTES];
        crypto_scalarmult_ristretto255(decryptedPassword, inverse, encryptedPassword);

        unsigned char expectedHash[crypto_core_ristretto255_HASHBYTES];
        crypto_generichash(expectedHash, sizeof expectedHash, (const unsigned char *)password.data(), password.length(), NULL, 0);
        unsigned char expectedPoint[crypto_core_ristretto255_BYTES];
        crypto_core_ristretto255_from_hash(expectedPoint, expectedHash);

        CHECK(std::memcmp(expectedPoint, decryptedPassword, crypto_core_ristretto255_BYTES) == 0);
    }
}