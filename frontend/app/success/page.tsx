"use client";
import Container from "@mui/material/Container";
import { createTheme, ThemeProvider } from "@mui/material/styles";
import Link from "next/link";

// can change colors if we want
const theme = createTheme({
  palette: {
    primary: { main: "#2196f3" },
    secondary: { main: "#651fff" },
  },
});

export default function Success() {
  return (
    <ThemeProvider theme={theme}>
      <Container component="main" maxWidth="xs">
        <div className="bg-white p-6 md:mx-auto absolute top-1/2 left-1/2 transform -translate-x-1/2 -translate-y-1/2">
          <div className="text-center">
      
            <svg
              viewBox="0 0 24 24"
              className="text-green-600 w-16 h-16 mx-auto my-6"
            >
              <path
                fill="currentColor"
                d="M12,0A12,12,0,1,0,24,12,12.014,12.014,0,0,0,12,0Zm6.927,8.2-6.845,9.289a1.011,1.011,0,0,1-1.43.188L5.764,13.769a1,1,0,1,1,1.25-1.562l4.076,3.261,6.227-8.451A1,1,0,1,1,18.927,8.2Z"
              ></path>
            </svg>

            <h3 className="md:text-6xl text-base font-semibold text-center">
              Account Created!
            </h3>
            <div className="py-10 text-center">
              <Link
                href="/"
                className="px-12 bg-indigo-600 hover:bg-indigo-500 text-white font-semibold py-3"
              >
                GO BACK
              </Link>
            </div>
          </div>
        </div>
      </Container>
    </ThemeProvider>
  );
}
