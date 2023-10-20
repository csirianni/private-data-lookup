import random
import string
import csv


def generate_passwords(num_passwords, max_chars=24) -> set[str]:
    """Generate a set of passwords containing at least one lowercase letter,
    at least one uppercase letter, at least one digit, and one special character.

    :param num_passwords: the number of passwords generated
    :param max_chars: the maximum number of characters in a password
    :return: the set of generated passwords
    """
    MIN_CHARS = 10

    passwords = set()
    while len(passwords) < num_passwords:
        # select one lowercase
        password = random.choice(string.ascii_lowercase)
        # select one uppercase
        password += random.choice(string.ascii_uppercase)
        # select one digit
        password += random.choice(string.digits)
        # select one special symbol
        password += random.choice(string.punctuation)

        # generate other characters
        character_options = string.ascii_letters + string.digits
        length = random.randint(MIN_CHARS - len(password), max_chars - len(password))
        for _ in range(length):
            password += random.choice(character_options)

        # shuffle all characters
        password = "".join(random.shuffle(list(password)))

        if password not in passwords:
            passwords.add(password)

    return passwords


# def write_passwords(file_path, passwords):
#     with open(file_path, 'w', newline="") as file:
