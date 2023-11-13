/**
 * from frontend/app:
 * compile: tsc ./psi.ts
 * to run: node ./psi.js
 */

const sodium = require("libsodium-wrappers-sumo");

function hashToPoint(input: string): Uint8Array {
    const hash = sodium.crypto_generichash(
        sodium.crypto_core_ristretto255_HASHBYTES,
        sodium.from_string(input)
    );
    return sodium.crypto_core_ristretto255_from_hash(hash);
}

/**
 * 
 * @param input the string to be encrypted
 * @returns input with a secret key applied and the key's inverse
 */
export function applySeed(input: string): [Uint8Array, Uint8Array] {
    // generate random seed
    const seed = sodium.crypto_core_ristretto255_scalar_random();
    // get seed inverse
    const seedInverse = sodium.crypto_core_ristretto255_scalar_invert(seed);
    const point = hashToPoint(input);
    // apply seed
    const seededInput = sodium.crypto_scalarmult_ristretto255(
        seed,
        point
    );

    return [seededInput, seedInverse]
}

function PSI() {
    let password = "Password123!";
    // Find intersection of these two sets.
    let serverSet = [
        "Password",
        "Kinan",
        "Alice",
        "Password123!",
        "Patrick",
    ];

    // Client and Server come up with two secret seeds.
    const b = sodium.crypto_core_ristretto255_scalar_random();

    // Client phase 1 - applies seed A to user's password
    // (client password)^a
    const [clientPasswordA, aInverse] = applySeed(password);
    // End of Client phase 1.

    // Server phase 1 - applies seed B to all breached passwords
    // (breached password)^b
    const serverSetB = serverSet.map(function (element) {
        const point = hashToPoint(element);
        return sodium.crypto_scalarmult_ristretto255(b, point);
    });
    // End of Server phase 1.

    // Server phase 2 - applies seed B to (user password)^a
    // (client password)^ab
    const clientPasswordAB = sodium.crypto_scalarmult_ristretto255(
        b,
        clientPasswordA
    );
    // End of Server phase 2.


    
    const options = new Set(
        serverSetB.map(function (element) {
            return element.join("");
        })
    );
    
    // Client phase 2 - applies inverse seed A to (user password)^ab
    // so now ((user password)^ab)^-a = (user password)^b
    const clientPasswordB = sodium.crypto_scalarmult_ristretto255(
        aInverse,
        clientPasswordAB
    );
    // End of Client phase 2.

    if (options.has(clientPasswordB.join(""))) {
        console.log(clientPasswordB.join(""));
    }
}

sodium.ready.then(PSI);
