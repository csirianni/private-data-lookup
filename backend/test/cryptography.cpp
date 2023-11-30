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
        unsigned char *encryptedPassword = cryptography::encryptPassword(password, b);

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

TEST_CASE("Test 2 encryptPassword")
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
        unsigned char *encryptedPassword = cryptography::encryptPassword(password, b);

        // unencrypt the password with the inverse of b
        unsigned char decryptedPassword[crypto_core_ristretto255_BYTES];
        crypto_scalarmult_ristretto255(decryptedPassword, inverse, encryptedPassword);

        unsigned char inverse2[crypto_core_ristretto255_SCALARBYTES];
        crypto_core_ristretto255_scalar_invert(inverse2, b);
        const std::string password2 = "TestPass1&";

        // encrypt password
        unsigned char *encryptedPassword2 = cryptography::encryptPassword(password2, b);

        // unencrypt the password with the inverse of b
        unsigned char decryptedPassword2[crypto_core_ristretto255_BYTES];
        crypto_scalarmult_ristretto255(decryptedPassword2, inverse2, encryptedPassword2);

        CHECK(std::memcmp(decryptedPassword, decryptedPassword2, crypto_core_ristretto255_BYTES) == 0);

        // compute expected value
        unsigned char expectedHash[crypto_core_ristretto255_HASHBYTES];
        crypto_generichash(expectedHash, sizeof expectedHash, (const unsigned char *)password.data(), password.length(), NULL, 0);
        unsigned char expectedPoint[crypto_core_ristretto255_BYTES];
        crypto_core_ristretto255_from_hash(expectedPoint, expectedHash);

        CHECK(std::memcmp(expectedPoint, decryptedPassword, crypto_core_ristretto255_BYTES) == 0);
        CHECK(std::memcmp(expectedPoint, decryptedPassword2, crypto_core_ristretto255_BYTES) == 0);
    }
}

TEST_CASE("Test encryptPassword fails WHYY")
{
    // generate constants
    unsigned char b[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random(b);
    unsigned char inverse[crypto_core_ristretto255_SCALARBYTES];
    unsigned char inverse2[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_invert(inverse, b);
    crypto_core_ristretto255_scalar_invert(inverse2, b);
    const std::string password = "TestPass1&";
    const std::string password2 = "TestPass1&";

    // encrypt password
    unsigned char *encryptedPassword = cryptography::encryptPassword(password, b);
    unsigned char *encryptedPassword2 = cryptography::encryptPassword(password2, b);

    // unencrypt the password with the inverse of b
    unsigned char decryptedPassword[crypto_core_ristretto255_BYTES];
    crypto_scalarmult_ristretto255(decryptedPassword, inverse, encryptedPassword);
    unsigned char decryptedPassword2[crypto_core_ristretto255_BYTES];
    crypto_scalarmult_ristretto255(decryptedPassword2, inverse2, encryptedPassword2);

    // compute expected value
    unsigned char expectedHash[crypto_core_ristretto255_HASHBYTES];
    crypto_generichash(expectedHash, sizeof expectedHash, (const unsigned char *)password.data(), password.length(), NULL, 0);
    unsigned char expectedPoint[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_from_hash(expectedPoint, expectedHash);

    CHECK(std::memcmp(expectedPoint, decryptedPassword, crypto_core_ristretto255_BYTES) == 0);
    CHECK(std::memcmp(expectedPoint, decryptedPassword2, crypto_core_ristretto255_BYTES) == 0);

    CHECK(std::memcmp(decryptedPassword, decryptedPassword2, crypto_core_ristretto255_BYTES) == 0);
}

TEST_CASE("Test encryptUserPassword")
{
    // generate constants
    unsigned char b[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_random(b);
    unsigned char inverse[crypto_core_ristretto255_SCALARBYTES];
    crypto_core_ristretto255_scalar_invert(inverse, b);
    unsigned char password[crypto_core_ristretto255_BYTES] = "TestPass1&";

    // encrypt password
    unsigned char *encryptedPassword = cryptography::encryptUserPassword(password, b);

    // unencrypt the password with the inverse of b
    unsigned char decryptedPassword[crypto_core_ristretto255_BYTES];
    crypto_scalarmult_ristretto255(decryptedPassword, inverse, encryptedPassword);

    // compute expected value
    unsigned char expectedHash[crypto_core_ristretto255_HASHBYTES];
    crypto_generichash(expectedHash, sizeof expectedHash, password, sizeof password, NULL, 0);
    unsigned char expectedPoint[crypto_core_ristretto255_BYTES];
    crypto_core_ristretto255_from_hash(expectedPoint, expectedHash);

    CHECK(std::memcmp(expectedPoint, decryptedPassword, crypto_core_ristretto255_BYTES) == 0);
}