import random
import string
import csv

'''
Generates 20 passwords between 10-24 characters that contain:
- at least 1 lowercase letter
- at least 1 uppercase letter
- at least 1 digit
- 1 special character

since when presented with these requirements, most users choose 1 special character and fill in the rest of the characters with letters and digits. Writes to passwords to a csv file.
'''
passwords, i = [], 0
while i < 20:
    # meet requirements:
    password = random.choice(string.ascii_lowercase) # select 1 lowercase
    password += random.choice(string.ascii_uppercase) # select 1 uppercase
    password += random.choice(string.digits) # select 1 digit
    password += random.choice(string.punctuation) # select 1 special symbol

    # generate other characters
    random_source = string.ascii_letters + string.digits
    r = random.randint(6,20)
    for _ in range(r):
        password += random.choice(random_source)

    # shuffle all characters
    password_list = list(password)
    random.SystemRandom().shuffle(password_list)
    password = ''.join(password_list)

    if password not in passwords:
        passwords.append(password)
        i += 1

# write to csv file
with open('./backend/database/breachedPasswords.csv', 'w', newline='') as csvfile:
    writer = csv.writer(csvfile, delimiter = '\n')
    writer.writerow(passwords)