import { applySeed, hashToPoint, checkSecurity } from "../app/psi";
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
    const inversedSeededPassword =
      sodium.crypto_scalarmult_ristretto255(
        seedInverse,
        seededPassword
      );

    // check that the seeded+inversed password is the same as the hashed password
    expect(
      Buffer.compare(inversedSeededPassword, hashedPassword)
    ).toBe(0);
  });
});

describe("testing expected server response", () => {
  test("sending breached password should return fail status", async () => {
    const password = "TestPass1&";
    const response = await checkSecurity(password);
    expect(response.status).toBe("fail");
  });
  test("sending non-breach password should return success status", async () => {
    const password = "NiniIsTheBest!4";
    const response = await checkSecurity(password);
    expect(response.status).toBe("success");
  });
});