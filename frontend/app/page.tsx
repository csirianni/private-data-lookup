"use client";
import {
    useState,
    useEffect,
    forwardRef,
    FormEvent,
    MouseEvent,
} from "react";
import Avatar from "@mui/material/Avatar";
import Button from "@mui/material/Button";
import CssBaseline from "@mui/material/CssBaseline";
import TextField from "@mui/material/TextField";
import Grid from "@mui/material/Grid";
import Box from "@mui/material/Box";
import Typography from "@mui/material/Typography";
import Container from "@mui/material/Container";
import IconButton from "@mui/material/IconButton";
import InputAdornment from "@mui/material/InputAdornment";
import LockOutlinedIcon from "@mui/icons-material/LockOutlined";
import CheckIcon from "@mui/icons-material/Check";
import CloseIcon from "@mui/icons-material/Close";
import Visibility from "@mui/icons-material/Visibility";
import VisibilityOff from "@mui/icons-material/VisibilityOff";
import { createTheme, ThemeProvider } from "@mui/material/styles";
import Snackbar, { SnackbarOrigin } from "@mui/material/Snackbar";
import MuiAlert, { AlertProps } from "@mui/material/Alert";
import CircularProgress from "@mui/material/CircularProgress";
import { useRouter } from "next/navigation";
import { checkSecurity } from "./handlers";
import PasswordChecklist from "react-password-checklist";

// can change colors if we want
const theme = createTheme({
    palette: {
        primary: { main: "#2196f3" },
        secondary: { main: "#651fff" },
    },
});

interface SnackbarState extends SnackbarOrigin {
    open: boolean;
}

export default function SignUp() {
    const router = useRouter();
    const [showPassword, setShowPassword] = useState<boolean>(false);
    const [isValidPassword, setIsValidPassword] = useState<boolean>(false);
    const [isValidForm, setIsValidForm] = useState<boolean>(false);
    const [first, setFirst] = useState<string>("");
    const [last, setLast] = useState<string>("");
    const [email, setEmail] = useState<string>("");
    const [password, setPassword] = useState<string>("");
    const [confirmPassword, setConfirmPassword] = useState<string>("");
    const [isLoading, setIsLoading] = useState<boolean>(false);
    const [errorText, setErrorText] = useState<string>("");
    const [showSnackbar, setShowSnackbar] = useState<SnackbarState>({
        vertical: "top",
        horizontal: "center",
        open: false,
    });
    const { vertical, horizontal, open } = showSnackbar;

    const handleClose = () => {
        setShowSnackbar({ ...showSnackbar, open: false });
    };

    const handleSubmit = async (
        event: FormEvent<HTMLFormElement>
    ) => {
        event.preventDefault();
        setIsLoading(true);
        const response = await checkSecurity(password); // makes an API call with the user's password
        setIsLoading(false);

        if (response.status == "success") {
            router.push(`/success`);
        } else if (response.status == "fail") {
            setErrorText(
                "Password was found in a previously breached dataset! Please try a different password."
            );
            setShowSnackbar({ ...showSnackbar, open: true });
        } else {
            setErrorText("Server error: please try again later");
            setShowSnackbar({ ...showSnackbar, open: true });
        }
    };

    const handleClickShowPassword = () =>
        setShowPassword((show) => !show);

    const handleMouseDownPassword = (
        event: MouseEvent<HTMLButtonElement>
    ) => {
        event.preventDefault();
    };

    useEffect(() => {
        setIsValidForm(first.length > 0 && last.length > 0 && email.length > 0);
    }, [first, last, email]);

    const Alert = forwardRef<HTMLDivElement, AlertProps>(
        function Alert(props, ref) {
            return (
                <MuiAlert
                    elevation={6}
                    ref={ref}
                    variant="filled"
                    {...props}
                />
            );
        }
    );

    return (
        <ThemeProvider theme={theme}>
            <Container component="main" maxWidth="xs">
                <CssBaseline />
                <Box
                    sx={{
                        marginTop: 8,
                        display: "flex",
                        flexDirection: "column",
                        alignItems: "center",
                    }}
                >
                    <Avatar sx={{ m: 1, bgcolor: "secondary.main" }}>
                        <LockOutlinedIcon />
                    </Avatar>
                    <Typography
                        aria-label="sign up header"
                        component="h1"
                        variant="h5"
                    >
                        Sign up
                    </Typography>
                    <Typography
                        aria-label="subheader"
                        component="p"
                        variant="subtitle1"
                        className="pt-2 text-gray-500 text-xs"
                    >
                        * Fields are required
                    </Typography>
                    <Box
                        id="sign up form"
                        component="form"
                        noValidate
                        onSubmit={handleSubmit}
                        sx={{ mt: 3 }}
                    >
                        <Grid container spacing={2}>
                            <Grid item xs={12} sm={6}>
                                <TextField
                                    autoComplete="given-name"
                                    name="firstName"
                                    required
                                    fullWidth
                                    id="firstName"
                                    label="First Name"
                                    aria-label="first name"
                                    onChange={(event) =>
                                        setFirst(event.target.value)
                                    }
                                    autoFocus
                                />
                            </Grid>
                            <Grid item xs={12} sm={6}>
                                <TextField
                                    required
                                    fullWidth
                                    id="lastName"
                                    label="Last Name"
                                    name="lastName"
                                    aria-label="last name"
                                    autoComplete="family-name"
                                    onChange={(event) =>
                                        setLast(event.target.value)
                                    }
                                />
                            </Grid>
                            <Grid item xs={12}>
                                <TextField
                                    required
                                    fullWidth
                                    id="email"
                                    label="Email Address"
                                    name="email"
                                    aria-label="email address"
                                    autoComplete="email"
                                    onChange={(event) =>
                                        setEmail(event.target.value)
                                    }
                                />
                            </Grid>
                            <Grid item xs={12}>
                                <TextField
                                    required
                                    fullWidth
                                    name="password"
                                    label="Password"
                                    id="password"
                                    autoComplete="new-password"
                                    aria-label="password"
                                    type={
                                        showPassword
                                            ? "text"
                                            : "password"
                                    }
                                    onChange={(event) =>
                                        setPassword(event.target.value)
                                    }
                                    InputProps={{
                                        endAdornment: (
                                            <InputAdornment position="end">
                                                <IconButton
                                                    aria-label="toggle password visibility"
                                                    onClick={handleClickShowPassword}
                                                    onMouseDown={handleMouseDownPassword}
                                                >
                                                    {showPassword ? (
                                                        <Visibility />
                                                    ) : (
                                                        <VisibilityOff />
                                                    )}
                                                </IconButton>
                                            </InputAdornment>
                                        ),
                                    }}
                                />
                            </Grid>
                            <Grid item xs={12}>
                                <TextField
                                    required
                                    fullWidth
                                    name="password"
                                    label="Confirm your password"
                                    id="confirm-password"
                                    aria-label="confirm password"
                                    type={
                                        showPassword
                                            ? "text"
                                            : "password"
                                    }
                                    onChange={(event) =>
                                        setConfirmPassword(event.target.value)
                                    }
                                    InputProps={{
                                        endAdornment: (
                                            <InputAdornment position="end">
                                                <IconButton
                                                    aria-label="toggle password visibility"
                                                    onClick={handleClickShowPassword}
                                                    onMouseDown={handleMouseDownPassword}
                                                >
                                                    {showPassword ? (
                                                        <Visibility />
                                                    ) : (
                                                        <VisibilityOff />
                                                    )}
                                                </IconButton>
                                            </InputAdornment>
                                        ),
                                    }}
                                />
                            </Grid>
                            <Typography
                                aria-label="password requirements"
                                component="div"
                                variant="inherit"
                                className="p-4"
                            >
                                <PasswordChecklist
                                    rules={[
                                        "minLength",
                                        "lowercase",
                                        "capital",
                                        "number",
                                        "specialChar",
                                        "match",
                                    ]}
                                    messages={{
                                        specialChar: "Password has a special character.",
                                    }}
                                    minLength={10}
                                    value={password}
                                    valueAgain={confirmPassword}
                                    onChange={(isValid) =>
                                        setIsValidPassword(isValid)
                                    }
                                    iconComponents={{
                                        ValidIcon: (
                                            <CheckIcon className="text-green-500 pt-1" />
                                        ),
                                        InvalidIcon: (
                                            <CloseIcon className="opacity-70 pt-1" />
                                        ),
                                    }}
                                />
                            </Typography>
                        </Grid>
                        <Button
                            aria-label="sign up button"
                            type="submit"
                            fullWidth
                            variant="contained"
                            sx={{ mt: 3, mb: 2 }}
                            disabled={
                                !isValidPassword ||
                                !isValidForm ||
                                isLoading
                            }
                        >
                            {isLoading ? (
                                <span className="inline-flex">
                                    <CircularProgress
                                        size="24px"
                                        color="inherit"
                                        sx={{ marginRight: "12px" }}
                                    />
                                    Loading...
                                </span>
                            ) : (
                                "Sign Up"
                            )}
                        </Button>
                    </Box>
                </Box>
                <Snackbar
                    anchorOrigin={{ vertical, horizontal }}
                    open={open}
                    key={vertical + horizontal}
                    autoHideDuration={8000}
                    onClose={handleClose}
                >
                    <Alert
                        severity="error"
                        onClose={handleClose}
                        sx={{ width: "100%" }}
                    >
                        {errorText}
                    </Alert>
                </Snackbar>
            </Container>
        </ThemeProvider>
    );
}
