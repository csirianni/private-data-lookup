# Backend

## About

The backend serves encrypted breached passwords using the REST API architecture. There are two available endpoints:

1. `/` (GET): This endpoint confirms the server is up and running.
2. `/breachedPasswords` (POST): This endpoint provided the encrypted breached passwords. The POST request should include the leaked bytes followed by the user's encrypted password. In general, for `n` leaked bytes, the length of the password is `32 + n` leaked bytes:

```text
0         n      32 + n
+---- ----+---- ----+
|   ...   |   ...   |
+---- ----+---- ----+
^leak     ^password
```

## Configuration

In `/backend`, start by installing [Conan](https://conan.io/):

```bash
brew install conan cmake
conan install . --output-folder=build --build=missing
```

You probably need to create a default profile. Use `conan profile detect`.

Next, link and compile the program:

```bash
make build
```

From `/backend`, start the server. Use the `--build` flag to create or rebuild a database for the breached passwords or omit it to use an existing one:

```bash
build/src/server <database filepath> --build
```

```bash
build/src/server data/passwords.db 
```

To fix VS Code import errors with Crow, try adding the following line to your `settings.json`:

```json
"C_Cpp.default.compileCommands": "${workspaceFolder}/build/compile_commands.json",
```

[Source](https://stackoverflow.com/questions/58077908/linking-conan-include-to-vs-code)

## Testing

After building, you can run tests from `/backend`:

```bash
cd build && ./test/pdl_test 
```

or alternatively:

```bash
make check
```
