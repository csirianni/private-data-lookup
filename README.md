# Private Data Lookup

[![Conventional Commits](https://github.com/csirianni/private-data-lookup/actions/workflows/commits.yml/badge.svg)](https://github.com/csirianni/private-data-lookup/actions/workflows/commits.yml)
[![Backend](https://github.com/csirianni/private-data-lookup/actions/workflows/backend.yml/badge.svg)](https://github.com/csirianni/private-data-lookup/actions/workflows/backend.yml)

## Overview

Private Data Lookup (PDL) is a web application that allows users to privately query a server-side data set of breached passwords during account creation. PDL is based on a secure multiparty computation mechanism called Private Set Intersection where two parties holdings sets (of credentials) can compute the intersection of the two sets without revealing their sets to the counterpart. The result is better application security by encouraging the use of stronger passwords and better user privacy by minimizing the information revealed to the server in the password precheck process.

> This project is inspired by some excellent work at Meta and Google:
>
> - [How Meta is improving password security and preserving privacy](https://engineering.fb.com/2023/08/08/security/how-meta-is-improving-password-security-and-preserving-privacy/)
> - [Data Breaches, Phishing, or Malware?: Understanding the Risks of Stolen Credentials](https://dl.acm.org/doi/10.1145/3133956.3134067)

## Implementation

### PSI

In Private Set Intersection, neither party reveals anything to their counterpart except for the elements in the intersection. This is accomplished using encryption. Hashed passwords are encrypted using secret key $a$ on the frontend and secret key $b$ on the backend. Querying the set of breached passwords is a three step process:

1. The client sends an encrypted user password $\text{Hash}(p)^a$ to the server.
2. The server sends the re-encrypted user password $\text{Hash}(p)^{ab}$ and the encrypted breached passwords $\text{Hash}(b_1)^{b}, ...,\text{Hash}(b_n)^{b}$ to the client.
3. The client partially decrypts the user password using $a^{-1}$ and checks if $\text{Hash}(p)^{aba^{-1}}$ is contained in the set of breached passwords.

If the set intersection is non-empty, the user's password is compromised and should not be used.

![PSI computation](https://private-user-images.githubusercontent.com/54677998/292687569-4fa846b9-1362-470c-b9cc-3de275a41706.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTEiLCJleHAiOjE3MDM0NDgyMjcsIm5iZiI6MTcwMzQ0NzkyNywicGF0aCI6Ii81NDY3Nzk5OC8yOTI2ODc1NjktNGZhODQ2YjktMTM2Mi00NzBjLWI5Y2MtM2RlMjc1YTQxNzA2LnBuZz9YLUFtei1BbGdvcml0aG09QVdTNC1ITUFDLVNIQTI1NiZYLUFtei1DcmVkZW50aWFsPUFLSUFJV05KWUFYNENTVkVINTNBJTJGMjAyMzEyMjQlMkZ1cy1lYXN0LTElMkZzMyUyRmF3czRfcmVxdWVzdCZYLUFtei1EYXRlPTIwMjMxMjI0VDE5NTg0N1omWC1BbXotRXhwaXJlcz0zMDAmWC1BbXotU2lnbmF0dXJlPTljY2RjMjQ0NTAxOWFmMTgxYjdhOGE1MGQ3YWUwYjQzZGQyYjIyNzY3Y2FlYjUxZTI2NjBhZGQ4YjRkZTY4ZWEmWC1BbXotU2lnbmVkSGVhZGVycz1ob3N0JmFjdG9yX2lkPTAma2V5X2lkPTAmcmVwb19pZD0wIn0.okTTuRlv7eqpOtsC9B0yABMfDWpIEydPFEz3LuRZYCw)

## Instructions

It's necessary to configure the `/frontend` and `/backend` folders initially. See the respective `README.md`s for more information. After configuration, you can run the application using the following commands.

To run the frontend, `cd` into `/frontend` and run

```console
yarn dev 
```

To run the backend, `cd` into the `/backend` and compile the program:

```console
make build
```

Then, you can simply run this command on an existing database:
Database file paths should end in .db, like `passwords.db`.

```console
build/src/server <database filepath>
```

If you want to build a new database from a new or existing path, you can use the `--build` flag after the path.

```console
build/src/server <database filepath> --build
```

Ensure that the backend is running with the frontend, otherwise you will see a server error on the front-end website.
