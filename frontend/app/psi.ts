const sodium = require("libsodium-wrappers-sumo");
// const { Print } = require("./util/util.js");

function hashToPoint(input: string) {
    const hash = sodium.crypto_generichash(
        sodium.crypto_core_ristretto255_HASHBYTES,
        sodium.from_string(input)
    );
    return sodium.crypto_core_ristretto255_from_hash(hash);
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
    const a = sodium.crypto_core_ristretto255_scalar_random();
    const aInverse = sodium.crypto_core_ristretto255_scalar_invert(a);
    const b = sodium.crypto_core_ristretto255_scalar_random();

    // Client phase 1 - applies seed A to user's password
    // (client password)^a
    const point = hashToPoint(password);
    const clientPasswordA = sodium.crypto_scalarmult_ristretto255(
        a,
        point
    );
    // End of Client phase 1.

    // Server phase 1 - applies seed B to all breached passwords
    // (breached password)^b
    const serverSetB = serverSet.map(function (element) {
        const point = hashToPoint(element);
        return sodium.crypto_scalarmult_ristretto255(b, point);
    });
    // End of Server phase 1.

    // Client and Server now exchange seeded sets.

    // Server phase 2 - applies seed B to (user password)^a
    // (client password)^ab
    const clientPasswordAB = sodium.crypto_scalarmult_ristretto255(
        b,
        clientPasswordA
    );
    // End of Server phase 2.

    // One more round of exchanging sets.
    // Say we are Alice, we now have access to bobSet2 and aliceSet2.
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
