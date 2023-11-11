import {
    crypto_pwhash_str,
    crypto_pwhash_str_verify,
    crypto_pwhash_OPSLIMIT_INTERACTIVE,
    crypto_pwhash_MEMLIMIT_INTERACTIVE,
} from "libsodium-wrappers-sumo";

// Make API call to server to check if password was found in breached dataset
export const checkSecurity = async (password: string) => {
    try {
        const response = await fetch(
            "http://localhost:18080/intersection",
            {
                method: "POST",
                mode: "cors",
                headers: {
                    "Access-Control-Allow-Headers": "*", // cors setting
                    "Content-Type": "application/json",
                },
                body: password,
            }
        );
        const data = await response.json();
        return data;
    } catch (error) {
        console.error("Error fetching data:", error);
        return { status: "error" };
    }
};

// Encrypt password using libsodium's ristretto.
function encrypt(password: string) {
    return;
}
