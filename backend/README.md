# Backend

## About

The backend serves encrypted breached passwords using the REST API architecture. There is a single endpoint:

### Create user password and get breached passwords

**POST** `/breachedPasswords`

This endpoint encrypts the user's password and provides a list of encrypted breached passwords.

#### Parameters

`body` *Required*

The leaked bytes followed by the user's encrypted password. In general, for `n` leaked bytes, the length of the password is `32 + n`:

```text
0         n         32 + n
+---- ----+---- ----+
|   ...   |   ...   |
+---- ----+---- ----+
^leak     ^password
```

The body must be [Base64](https://en.wikipedia.org/wiki/Base64) encoded.

#### Response

| Key                 | Description                                            | Always present |
|---------------------|--------------------------------------------------------|----------------|
| `status`            | A string, `success`, `fail` or `error`.                | Yes            |
| `userPassword`      | A string, the user's password encrypted.               | No             |
| `breachedPasswords` | An array of strings, the breached passwords encrypted. | No             |

The user password and breached passwords are Base64 encoded.

#### Example

```typescript
import { base64 } from "rfc4648";

const response = await fetch(
    "http://localhost:18080/breachedPasswords",
    {
        method: "POST",
        mode: "cors",
        headers: {
            "Access-Control-Allow-Headers": "*",
            "Content-Type": "application/json",
        },
        body: base64.stringify(password),
    }
);
```

## Configuration

In `/backend`, start by installing [Conan](https://conan.io/):

```bash
brew install conan 
conan install . --output-folder=build --build=missing
```

You probably need to create a default profile. Use `conan profile detect`.

If you haven't installed CMake already, do so now:

```bash
brew install cmake
```

Next, link and compile the program:

```bash
make build
```

From `/backend`, start the server. Use the `--build` flag to create or rebuild a database for the breached passwords:

```bash
build/src/server <database filepath> --build
```

Or, omit the `--build` flag to use an existing database:

```bash
build/src/server data/passwords.db 
```

## Debugging

To fix VS Code import errors, try adding the following line to your `settings.json`:

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
