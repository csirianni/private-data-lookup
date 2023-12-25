# Frontend

## About

The frontend renders the client web application and computes the private set intersection. It is made with [Next.js](https://nextjs.org/), [Tailwind](https://tailwindcss.com/), and [MaterialUI](https://mui.com/material-ui/).

![Sign up page](https://i.imgur.com/8sea2io.png)

Note that a password must satisfy the listed requirements before the user can click "Sign Up."

## Configuration

Make sure you have [Yarn](https://yarnpkg.com/) and [Node](https://nodejs.org/en) installed.

To run the frontend server, use your preferred terminal to `cd` into `/frontend` and then install the required packages by running

```bash
yarn install
```

Then, you should be able to start the server with the command

```bash
yarn dev
```

Open [http://localhost:3000](http://localhost:3000) to view it in the browser. The page will reload if you make edits.

## Testing

You can run tests from `/frontend`:

```console
yarn test
```

This command involves integration testing, so make sure the backend is running and that the leaked bytes are the same. For example,

`/frontend/tests/psi.test.ts`

```javascript
test("sending breached password should return fail status", async () => {
    const password = "TestPass1&";
    const response = await checkSecurity(password, 1);
    expect(response.status).toBe("fail");
});
```

```/backend/src/main.cpp```

```cpp
const size_t offset = 1;
```
