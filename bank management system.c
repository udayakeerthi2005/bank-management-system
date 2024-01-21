#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a bank account
struct BankAccount {
    int accountNumber;
    char holderName[100];
    float balance;
};

// Function declarations
struct BankAccount createAccount(int accountNumber, char holderName[], float initialBalance);
int accountExists(int accountNumber);
void addAccount(struct BankAccount account);
void displayAccountDetails(struct BankAccount account);
void updateAccount(struct BankAccount *account);
void deleteAccount(int accountNumber);
void browseCustomers();

// Function to create a new account
struct BankAccount createAccount(int accountNumber, char holderName[], float initialBalance) {
    struct BankAccount newAccount;
    newAccount.accountNumber = accountNumber;
    strcpy(newAccount.holderName, holderName);
    newAccount.balance = initialBalance;
    return newAccount;
}

// Function to check if an account exists in the file
int accountExists(int accountNumber) {
    FILE *file = fopen("accounts.dat", "rb");
    if (file == NULL) {
        return 0; // File does not exist
    }
    struct BankAccount account;
    while (fread(&account, sizeof(struct BankAccount), 1, file) == 1) {
        if (account.accountNumber == accountNumber) {
            fclose(file);
            return 1; // Account exists
        }
    }

    fclose(file);
    return 0; // Account does not exist
}

// Function to add a new account to the file
void addAccount(struct BankAccount account) {
    FILE *file = fopen("accounts.dat", "ab");
    if (file != NULL) {
        fwrite(&account, sizeof(struct BankAccount), 1, file);
        fclose(file);
        printf("Account created successfully.\n");
    } else {
        printf("Error creating account.\n");
    }
}

// Function to display account details
void displayAccountDetails(struct BankAccount account) {
    printf("\nAccount Number: %d\n", account.accountNumber);
    printf("Account Holder: %s\n", account.holderName);
    printf("Balance: %.2f\n", account.balance);
}

// Function to update account information
void updateAccount(struct BankAccount *account) {
    printf("Enter new account holder name: ");
    scanf("%s", account->holderName);
}

// Function to delete an existing account
void deleteAccount(int accountNumber) {
    FILE *inputFile = fopen("accounts.dat", "rb");
    FILE *tempFile = fopen("temp.dat", "wb");

    if (inputFile == NULL || tempFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    struct BankAccount account;
    while (fread(&account, sizeof(struct BankAccount), 1, inputFile) == 1) {
        if (account.accountNumber != accountNumber) {
            fwrite(&account, sizeof(struct BankAccount), 1, tempFile);
        }
    }

    fclose(inputFile);
    fclose(tempFile);

    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");

    printf("Account deleted successfully.\n");
}

// Function to display a list of all customers
void browseCustomers() {
    FILE *file = fopen("accounts.dat", "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    struct BankAccount account;
    printf("\nList of Customers:\n");
    while (fread(&account, sizeof(struct BankAccount), 1, file) == 1) {
        displayAccountDetails(account);
    }

    fclose(file);
}

int main() {
    int choice;
    int accountNumber;

    do {
        printf("\nBanking Management System\n");
        printf("1. Establish a new account\n");
        printf("2. Change existing account information\n");
        printf("3. See and manage transactions\n");
        printf("4. Verify details of an existing account\n");
        printf("5. Delete an existing account\n");
        printf("6. Browse list of customers\n");
        printf("7. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // Establish a new account
                printf("Enter account number: ");
                scanf("%d", &accountNumber);

                // Check if account already exists
                if (accountExists(accountNumber)) {
                    printf("Account already exists.\n");
                    break;
                }

                char holderName[100];
                float initialBalance;
                printf("Enter account holder name: ");
                scanf("%s", holderName);

                printf("Enter initial balance: ");
                scanf("%f", &initialBalance);

                // Create a new account
                struct BankAccount newAccount = createAccount(accountNumber, holderName, initialBalance);

                // Add the new account to the file
                addAccount(newAccount);
                break;

            case 2:
                // Change existing account information
                printf("Enter account number: ");
                scanf("%d", &accountNumber);

                // Check if account exists
                if (!accountExists(accountNumber)) {
                    printf("Account does not exist.\n");
                    break;
                }

                // Update account information
                FILE *file = fopen("accounts.dat", "rb+");
                if (file == NULL) {
                    printf("Error opening file.\n");
                    break;
                }

                struct BankAccount accountToUpdate;
                while (fread(&accountToUpdate, sizeof(struct BankAccount), 1, file) == 1) {
                    if (accountToUpdate.accountNumber == accountNumber) {
                        updateAccount(&accountToUpdate);
                        fseek(file,-sizeof(struct BankAccount), SEEK_CUR);
                        fwrite(&accountToUpdate, sizeof(struct BankAccount), 1, file);
                        break;
                    }
                }

                fclose(file);
                printf("Account information updated successfully.\n");
                break;

            case 3:
                // See and manage transactions (Not implemented in this example)
                printf("This feature is not implemented in this example.\n");
                break;

            case 4:
                // Verify details of an existing account
                printf("Enter account number: ");
                scanf("%d", &accountNumber);

                // Check if account exists
                if (!accountExists(accountNumber)) {
                    printf("Account does not exist.\n");
                    break;
                }

                // Display account details
                file = fopen("accounts.dat", "rb");
                if (file != NULL) {
                    struct BankAccount accountDetails;
                    while (fread(&accountDetails, sizeof(struct BankAccount), 1, file) == 1) {
                        if (accountDetails.accountNumber == accountNumber) {
                            displayAccountDetails(accountDetails);
                            break;
                        }
                    }
                    fclose(file);
                }
                break;

            case 5:
                // Delete an existing account
                printf("Enter account number to delete: ");
                scanf("%d", &accountNumber);

                // Check if account exists
                if (!accountExists(accountNumber)) {
                    printf("Account does not exist.\n");
                    break;
                }

                // Delete the account
                deleteAccount(accountNumber);
                break;

            case 6:
                // Browse list of customers
                browseCustomers();
                break;

            case 7:
                // Exit
                printf("Exiting the program.\n");
                break;

            default:
                printf("Invalid choice. Please enter a number between 1 and 7.\n");
        }

    } while (choice != 7);

    return 0;
}
