"use strict";
/**
 * from frontend/app:
 * compile: tsc ./psi.ts
 * to run: node ./psi.js
 */
Object.defineProperty(exports, "__esModule", { value: true });
exports.applySeed = void 0;
var sodium = require("libsodium-wrappers-sumo");
function hashToPoint(input) {
    var hash = sodium.crypto_generichash(sodium.crypto_core_ristretto255_HASHBYTES, sodium.from_string(input));
    return sodium.crypto_core_ristretto255_from_hash(hash);
}
/**
 *
 * @param input the string to be encrypted
 * @returns input with a secret key applied and the key's inverse
 */
function applySeed(input) {
    // generate random seed
    var seed = sodium.crypto_core_ristretto255_scalar_random();
    // get seed inverse
    var seedInverse = sodium.crypto_core_ristretto255_scalar_invert(seed);
    var point = hashToPoint(input);
    // apply seed
    var seededInput = sodium.crypto_scalarmult_ristretto255(seed, point);
    return [seededInput, seedInverse];
}
exports.applySeed = applySeed;
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
    var b = sodium.crypto_core_ristretto255_scalar_random();
    // Client phase 1 - applies seed A to user's password
    // (client password)^a
    var _a = applySeed(password), clientPasswordA = _a[0], aInverse = _a[1];
    // End of Client phase 1.
    // Server phase 1 - applies seed B to all breached passwords
    // (breached password)^b
    var serverSetB = serverSet.map(function (element) {
        var point = hashToPoint(element);
        return sodium.crypto_scalarmult_ristretto255(b, point);
    });
    // End of Server phase 1.
    // Server phase 2 - applies seed B to (user password)^a
    // (client password)^ab
    var clientPasswordAB = sodium.crypto_scalarmult_ristretto255(b, clientPasswordA);
    // End of Server phase 2.
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
