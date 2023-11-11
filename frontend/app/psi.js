var sodium = require("libsodium-wrappers-sumo");
// const { Print } = require("./util/util.js");
function hashToPoint(input) {
    var hash = sodium.crypto_generichash(sodium.crypto_core_ristretto255_HASHBYTES, sodium.from_string(input));
    return sodium.crypto_core_ristretto255_from_hash(hash);
}
function PSI() {
    var password = "Password123!";
    // Find intersection of these two sets.
    var serverSet = [
        "Password",
        "Kinan",
        "Alice",
        "Password123!",
        "Patrick",
    ];
    // Client and Server come up with two secret seeds.
    var a = sodium.crypto_core_ristretto255_scalar_random();
    var aInverse = sodium.crypto_core_ristretto255_scalar_invert(a);
    var b = sodium.crypto_core_ristretto255_scalar_random();
    // Client phase 1 - applies seed A to user's password
    // (client password)^a
    var point = hashToPoint(password);
    var clientPasswordA = sodium.crypto_scalarmult_ristretto255(a, point);
    // End of Client phase 1.
    // Server phase 1 - applies seed B to all breached passwords
    // (breached password)^b
    var serverSetB = serverSet.map(function (element) {
        var point = hashToPoint(element);
        return sodium.crypto_scalarmult_ristretto255(b, point);
    });
    // End of Server phase 1.
    // Client and Server now exchange seeded sets.
    // Server phase 2 - applies seed B to (user password)^a
    // (client password)^ab
    var clientPasswordAB = sodium.crypto_scalarmult_ristretto255(b, clientPasswordA);
    // End of Server phase 2.
    // One more round of exchanging sets.
    // Say we are Alice, we now have access to bobSet2 and aliceSet2.
    var options = new Set(serverSetB.map(function (element) {
        return element.join("");
    }));
    // Client phase 2 - applies inverse seed A to (user password)^ab
    // so now ((user password)^ab)^-a = (user password)^b
    var clientPasswordB = sodium.crypto_scalarmult_ristretto255(aInverse, clientPasswordAB);
    // End of Client phase 2.
    if (options.has(clientPasswordB.join(""))) {
        console.log(clientPasswordB.join(""));
    }
}
sodium.ready.then(PSI);
