import { base64 } from "rfc4648";

const sodium = require("libsodium-wrappers-sumo");
const fetch = require("node-fetch");

type ServerResponse = {
    userPassword: string;
    breachedPasswords: string[];
};

/**
 * Hash a password to a point on an elliptic curve 
 * @param input the password to be hashed to a point on an elliptic curve
 * @returns the point on an elliptic curve
 */
export function hashToPoint(input: string): Uint8Array {
    const hash = sodium.crypto_generichash(
        sodium.crypto_core_ristretto255_HASHBYTES,
        sodium.from_string(input),
    );
    return sodium.crypto_core_ristretto255_from_hash(hash);
}

/**
 * Apply a random seed to the password and returns the encrypted password and the inverse of the seed 
 * @param input the password to be encrypted
 * @param offset the number of bytes to be leaked
 * @returns the encrypted password and the inverse of the seed
 */
export function applySeed(input: string, offset = 0): [Uint8Array, Uint8Array] {
    // generate random seed
    const seed = sodium.crypto_core_ristretto255_scalar_random();
    // get seed inverse
    const seedInverse =
        sodium.crypto_core_ristretto255_scalar_invert(seed);
    const point = hashToPoint(input);
    // apply seed
    const leakedBytes = point.subarray(0, offset);
    const seededPassword = sodium.crypto_scalarmult_ristretto255(
        seed,
        point
    );
    const leakedSeededPassword = new Uint8Array(offset + seededPassword.length);
    leakedSeededPassword.set(leakedBytes, 0);
    leakedSeededPassword.set(seededPassword, offset);
    return [leakedSeededPassword, seedInverse];
}

function computeIntersection(
    data: ServerResponse,
    aInverse: Uint8Array,
    offset = 0
): boolean {
    const userPassword = base64.parse(data.userPassword);
    const breachedPasswords = new Set((data.breachedPasswords).map(function (element) { return base64.parse(element).join(""); }));

    // Client phase 2 - applies inverse seed A to (user password)^ab
    // so now ((user password)^ab)^-a = (user password)^b
    const clientPasswordB = sodium.crypto_scalarmult_ristretto255(
        aInverse,
        userPassword
    );
    // End of Client phase 2.

    return breachedPasswords.has(clientPasswordB.join(""));
}

// Make API call to server to check if password was found in breached dataset
export async function checkSecurity(password: string, offset = 0) {
    try {
        const [seededPassword, keyInverse] = applySeed(password, offset);

        const response = await fetch(
            "http://localhost:18080/breachedPasswords",
            {
                method: "POST",
                mode: "cors",
                headers: {
                    "Access-Control-Allow-Headers": "*", // cors setting
                    "Content-Type": "application/json",
                },
                body: base64.stringify(seededPassword),
            }
        );
        const data = await response.json();
        if (computeIntersection(data, keyInverse, offset)) {
            return { status: "fail" };
        } else {
            return { status: "success" };
        }
    } catch (error) {
        console.error("Error fetching data:", error);
        return { status: "error" };
    }
}
