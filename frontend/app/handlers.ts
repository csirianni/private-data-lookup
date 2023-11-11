
// 1. send post request with secret key A
// 2. await server response with {(client password)^ab, breachedPasswordSet^(b)}
// 3. apply inverse to (client password)^ab and create set from breachedPasswordSet^(b)
// 4. compute set intersection.

import { applySeed } from "./psi";

// Make API call to server to check if password was found in breached dataset
export async function checkSecurity(password: string) {
    try {
        const [seededPassword, keyInverse] = applySeed(password);

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
