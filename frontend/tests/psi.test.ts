import { applySeed, hashToPoint } from "../app/psi";
const sodium = require("libsodium-wrappers-sumo");

beforeAll(async () => {
  await sodium.ready;
});

describe("testing applySeed", () => {
  test("applying secret key should be same as original value", () => {
    const password = "TestPass1&";

    // hash the password
    const hashedPassword = hashToPoint(password);
    // apply the seed
    const [seededPassword, seedInverse] = applySeed(password);
    // apply the inverse to the seeded password
    const inversedSeededPassword = sodium.crypto_scalarmult_ristretto255(
      seedInverse,
      seededPassword
    );
    
    // check that the seeded+inversed password is the same as the hashed password
    expect(Buffer.compare(inversedSeededPassword, hashedPassword)).toBe(0);
  });
});
