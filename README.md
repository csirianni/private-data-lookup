# Private Data Lookup

[![Conventional Commits](https://github.com/csirianni/private-data-lookup/actions/workflows/commits.yml/badge.svg)](https://github.com/csirianni/private-data-lookup/actions/workflows/commits.yml)
[![Backend](https://github.com/csirianni/private-data-lookup/actions/workflows/backend.yml/badge.svg)](https://github.com/csirianni/private-data-lookup/actions/workflows/backend.yml)

## Overview

Private Data Lookup (PDL) is a web application that allows users to privately query a server-side data set of breached passwords during account creation. PDL is based on a secure multiparty computation mechanism called Private Set Intersection where two parties holdings sets (of credentials) can compute the intersection of the two sets without revealing their sets to the counterpart. The result is better application security by encouraging the use of stronger passwords and better user privacy by minimizing the information revealed to the server in the password precheck process.

> This project is inspired by some excellent work at Meta and Google:
>
> - [How Meta is improving password security and preserving privacy](https://engineering.fb.com/2023/08/08/security/how-meta-is-improving-password-security-and-preserving-privacy/)
> - [Data Breaches, Phishing, or Malware?: Understanding the Risks of Stolen Credentials](https://dl.acm.org/doi/10.1145/3133956.3134067)

## Instructions

It's necessary to configure the `/frontend` and `/backend` folders initially. See the respective `README.md`s for more information. After configuration, you can run the application using the following commands.

To run the frontend, `cd` into `/frontend` and run

```console
yarn dev 
```

To run the backend, `cd` into the `/backend` and compile the program:

```console
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

Then, you can simply run

```console
./src/server
```

Ensure that the backend is running with the frontend, otherwise you will 
see a message of server errors on the front-end website. 