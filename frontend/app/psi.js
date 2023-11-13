/**
 * from frontend/app:
 * compile: tsc ./psi.ts
 * to run: node ./psi.js
 */
var sodium = require("libsodium-wrappers-sumo");
function hashToPoint(input) {
    var hash = sodium.crypto_generichash(sodium.crypto_core_ristretto255_HASHBYTES, sodium.from_string(input));
    return sodium.crypto_core_ristretto255_from_hash(hash);
}
function PSI() {
    var password = "Password123!";
    var serverSet = [
        "Password",
        "Kinan",
        "Alice",
        "Password123!",
        "Patrick",
    ];
    // client and server come up with two secret seeds
    var b = sodium.crypto_core_ristretto255_scalar_random();
    var a = sodium.crypto_core_ristretto255_scalar_random();
    var point = hashToPoint(password);
    // client phase 1 - apply seed a to user's password
    // (client password)^a
    var clientPasswordA = sodium.crypto_scalarmult_ristretto255(a, point);
    // server phase 1 - apply seed b to all breached passwords
    // (breached password)^b
    var serverSetB = serverSet.map(function (element) {
        var point = hashToPoint(element);
        return sodium.crypto_scalarmult_ristretto255(b, point);
    });
    // server phase 2 - apply seed b to (user password)^a
    // (client password)^ab
    var clientPasswordAB = sodium.crypto_scalarmult_ristretto255(b, clientPasswordA);
    // combine bytes into single string
    var options = new Set(serverSetB.map(function (element) {
        return element.join("");
    }));
    var aInverse = sodium.crypto_core_ristretto255_scalar_invert(a);
    // Client phase 2 - applies inverse seed A to (user password)^ab
    // so now ((user password)^ab)^-a = (user password)^b
    var clientPasswordB = sodium.crypto_scalarmult_ristretto255(aInverse, clientPasswordAB);
    // compute private set intersection
    if (options.has(clientPasswordB.join(""))) {
        console.log(clientPasswordB.join(""));
    }
}
sodium.ready.then(PSI);
