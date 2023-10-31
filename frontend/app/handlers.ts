// Make API call to server to check if password was found in breached dataset
export const checkSecurity = async (password) => {
    try {
        const response = await fetch("http://localhost:18080/" + password, {
            method: "POST",
            mode: "cors",
            headers: {
                "Access-Control-Allow-Headers": "*",
            },
        })
        const data = await response.json();
        return data;
    } catch (error) {
        console.error("Error fetching data:", error);
        return { status: "error" };
    }
};
