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

### Optimization

Critical path latency in the system is defined as the time elapsed between the user clicking "Sign Up" and the browser specifying if the password is breached or not. To reduce critical path latency, [$k$-anonymity](https://en.wikipedia.org/wiki/K-anonymity) is used. By partitioning passwords into $k$ buckets, the number of passwords serialized by the server during each request is significantly reduced. This decreases the time to download breached passwords and linearly scan the set.

Passwords are assigned a bucket based on one or more leaked bytes. Given $n$ leaked bytes, there are $\left[0, (2^8)^n - 1\right]$ buckets. This feature involves a tradeoff between user privacy and application performance. The key assumption is that the number of breached passwords is sufficiently large to not reveal identifiable information about individual users. Since real breached password datasets contain billions of passwords [[1](https://www.wired.com/story/collection-leak-usernames-passwords-billions/)], this assumption is reasonable.

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
