# Private Data Lookup

[![Conventional Commits](https://github.com/csirianni/private-data-lookup/actions/workflows/commits.yml/badge.svg)](https://github.com/csirianni/private-data-lookup/actions/workflows/commits.yml)

## Overview

## Instructions

It's necessary to configure the `/frontend` and `/backend` folders initially. See the respective `README.md`s for more information. After configuration, you can run the application using the following commands.

To run the frontend, `cd` into `/frontend` and run

```console
yarn dev 
```

To run the backend, `cd` into the `/backend` and compile the executable:

```console
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

Then, you can simply run

```console
./backend
```
