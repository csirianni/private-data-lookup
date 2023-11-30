#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "cryptography.hpp"
#include "sodium.h"

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
        unsigned char* encryptedPassword = cryptography::encryptPassword(password, b);
        // unsigned char encryptedPassword[crypto_core_ristretto255_BYTES];
        // std::memcpy(encryptedPassword, encryptedPasswordStr.data(), crypto_core_ristretto255_BYTES);

        // unencrypt the password with the inverse of b
        unsigned char decryptedPassword[crypto_core_ristretto255_BYTES];
        crypto_scalarmult_ristretto255(decryptedPassword, inverse, encryptedPassword);

        // compute expected value
        unsigned char expectedHash[crypto_core_ristretto255_HASHBYTES];
        crypto_generichash(expectedHash, sizeof expectedHash, (const unsigned char *)password.data(), password.length(), NULL, 0);
        unsigned char expectedPoint[crypto_core_ristretto255_BYTES];
        crypto_core_ristretto255_from_hash(expectedPoint, expectedHash);

        CHECK(std::memcmp(expectedPoint, decryptedPassword, crypto_core_ristretto255_BYTES) == 0);
    }
}