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


function PSI() {
    let password = "Password123!";
    let serverSet = [
        "Password",
        "Kinan",
        "Alice",
        "Password123!",
        "Patrick",
    ];

    // generate constants including seeds for client and server
    const b = sodium.crypto_core_ristretto255_scalar_random();
    const a = sodium.crypto_core_ristretto255_scalar_random();
    const aInverse = sodium.crypto_core_ristretto255_scalar_invert(a);
    const point = hashToPoint(password);

    // client phase 1 - apply seed a to user's password
    // (client password)^a
    const clientPasswordA = sodium.crypto_scalarmult_ristretto255(
        a,
        point
    );

    // server phase 1 - apply seed b to all breached passwords
    // (breached password)^b
    const serverSetB = serverSet.map(function (element) {
        const point = hashToPoint(element);
        return sodium.crypto_scalarmult_ristretto255(b, point);
    });

    // server phase 2 - apply seed b to (user password)^a
    // (client password)^ab
    const clientPasswordAB = sodium.crypto_scalarmult_ristretto255(
        b,
        clientPasswordA
    );

    // combine bytes into single string
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

    // compute private set intersection
    if (options.has(clientPasswordB.join(""))) {
        console.log(clientPasswordB.join(""));
    }
}

sodium.ready.then(PSI);
