const sodium = require("libsodium-wrappers-sumo");

type ServerResponse = {
    userPassword: string;
    breachedPasswords: string[];
};

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
function applySeed(input: string): [ArrayBuffer, Uint8Array] {
    // generate random seed
    const seed = sodium.crypto_core_ristretto255_scalar_random();
    // get seed inverse
    const seedInverse =
        sodium.crypto_core_ristretto255_scalar_invert(seed);
    const point = hashToPoint(input);
    // apply seed
    const seededInput = sodium.crypto_scalarmult_ristretto255(
        seed,
        point
    );
    console.log(new TextDecoder().decode(seededInput.buffer));
    return [seededInput.buffer, seedInverse];
}

function computeIntersection(
    data: ServerResponse,
    aInverse: Uint8Array
): boolean {
    const password = data.userPassword;
    const serverSet = data.breachedPasswords;

    const options = new Set(
        serverSet.map(function (element) {
            return element;
        })
    );

    // Client phase 2 - applies inverse seed A to (user password)^ab
    // so now ((user password)^ab)^-a = (user password)^b
    const clientPasswordB = sodium.crypto_scalarmult_ristretto255(
        aInverse,
        sodium.from_string(password)
    );
    // End of Client phase 2.

    if (options.has(clientPasswordB.join(""))) {
        console.log(clientPasswordB.join(""));
        return true;
    }

    return false;
}

// Make API call to server to check if password was found in breached dataset
export async function checkSecurity(password: string) {
    try {
        const [seededPassword, keyInverse] = applySeed(password);

        const response = await fetch(
            "http://localhost:18080/breachedPasswords",
            {
                method: "POST",
                mode: "cors",
                headers: {
                    "Access-Control-Allow-Headers": "*", // cors setting
                    "Content-Type": "application/json",
                },
                body: seededPassword,
            }
        );
        const data = await response.json();
        if (computeIntersection(data, keyInverse)) {
            return { status: "success" };
        } else {
            return { status: "fail" };
        }
    } catch (error) {
        console.error("Error fetching data:", error);
        return { status: "error" };
    }
}
