#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Function to convert a string to uppercase
void convertToUpperCase(char* str) {
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

// Function to check if an account exists in the specified file
int checkAccount(char* filename, int accNo, char* name) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("File not found. Creating a new file...\n");
        file = fopen(filename, "w");
        if (file == NULL) {
            printf("Error creating file.\n");
            return 0;
        }
        fclose(file);
        return 0;
    }

    int fileAccNo;
    while (fscanf(file, "%d,%*[^,\n],%[^\n]", &fileAccNo, name) == 2) {
        if (fileAccNo == accNo) {
            fclose(file);
            return 1;
        }
        fseek(file, 1, SEEK_CUR);  // Skip the comma
    }

    fclose(file);
    return 0;
}

// Function to authenticate an account based on account number and PIN
int authenticateAccount(char* filename, int accNo, int pin, char* name) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    int fileAccNo, filePin;
    float balance;
    while (fscanf(file, "%d,%d,%f,%[^\n]", &fileAccNo, &filePin, &balance, name) == 4) {
        if (fileAccNo == accNo && filePin == pin) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

// Function to withdraw money from an account
void withdrawMoney(char* filename, int accNo, int pin, float amount) {
    float balance;
    int updatedBalance = 0;
    char name[50];

    if (authenticateAccount(filename, accNo, pin, name)) {
        FILE* file = fopen(filename, "r+");
        if (file == NULL) {
            printf("Error opening file.\n");
            return;
        }

        FILE* tempFile = fopen("temp.txt", "w");
        if (tempFile == NULL) {
            fclose(file);
            printf("Error creating temporary file.\n");
            return;
        }

        int fileAccNo, filePin;
        while (fscanf(file, "%d,%d,%f,%49[^\n]\n", &fileAccNo, &filePin, &balance, name) == 4) {
            if (fileAccNo == accNo) {
                if (balance >= amount) {
                    balance -= amount;
                    printf("          Withdrawal successful.\n          Remaining balance: %.2f\n", balance);
                    updatedBalance = 1;
                } else {
                    printf("Insufficient funds. Withdrawal failed.\n");
                }
            }
            fprintf(tempFile, "%d,%d,%.2f,%s\n", fileAccNo, filePin, balance, name);
        }

        fclose(file);
        fclose(tempFile);

        if (remove(filename) != 0) {
            printf("Error deleting the original file.\n");
            return;
        }

        if (rename("temp.txt", filename) != 0) {
            printf("Error renaming the temporary file.\n");
            return;
        }

        if (!updatedBalance) {
            printf("Account not found.\n");
        }
    } else {
        printf("Invalid account number or PIN. Withdrawal failed.\n");
    }
}

// Function to display account balance
void displayAccountBalance(char* filename, int accNo, char* name) {
    float balance;

    if (checkAccount(filename, accNo, name)) {
        FILE* file = fopen(filename, "r");
        if (file == NULL) {
            printf("Error opening file.\n");
            return;
        }

        int fileAccNo, filePin;
        while (fscanf(file, "%d,%d,%f,%49[^\n]\n", &fileAccNo, &filePin, &balance, name) == 4) {
            if (fileAccNo == accNo) {
                printf("Name: %s\n", name);
                printf("Account balance: %.2f\n", balance);
                fclose(file);
                return;
            }
        }

        fclose(file);
        printf("Account not found.\n");
    } else {
        printf("Account not found.\n");
    }
}

void transferMoney(char* filename, int senderAccNo, int senderPin, int receiverAccNo, float amount) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    int senderFound = 0;
    int receiverFound = 0;
    int fileAccNo, filePin;
    float senderBalance, receiverBalance;
    char senderName[50], receiverName[50];

    // Read the entire file into memory
    struct Account {
        int accNo;
        int pin;
        float balance;
        char name[50];
    };

    struct Account accounts[100];  // Assuming a maximum of 100 accounts
    int numAccounts = 0;

    while (fscanf(file, "%d,%d,%f,%49[^\n]\n", &fileAccNo, &filePin, &accounts[numAccounts].balance, accounts[numAccounts].name) == 4) {
        accounts[numAccounts].accNo = fileAccNo;
        accounts[numAccounts].pin = filePin;

        if (fileAccNo == senderAccNo) {
            if (accounts[numAccounts].balance >= amount) {
                accounts[numAccounts].balance -= amount;
                senderFound = 1;
                printf("\n\n          Transfer successful from %s's account.\n          Remaining balance: %.2f\n", accounts[numAccounts].name, accounts[numAccounts].balance);
            } else {
                printf("Insufficient funds. Transfer failed.\n");
                fclose(file);
                return;
            }
        }

        if (fileAccNo == receiverAccNo) {
            accounts[numAccounts].balance += amount;
            receiverFound = 1;
            printf("\n          Transfer successful to %s's account.\n          Receiver's new balance: %.2f\n", accounts[numAccounts].name, accounts[numAccounts].balance);
        }

        numAccounts++;
    }

    fclose(file);

    // Check if sender and receiver accounts were found
    if (!senderFound) {
        printf("Sender account not found.\n");
        return;
    }

    if (!receiverFound) {
        printf("Receiver account not found.\n");
        return;
    }

    // Write the modified data back to the file
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numAccounts; i++) {
        fprintf(file, "%d,%d,%.2f,%s\n", accounts[i].accNo, accounts[i].pin, accounts[i].balance, accounts[i].name);
    }

    fclose(file);
}

// Main function
int main() {
    char filename[] = "account_details.txt";

    int accNo, pin, option, receiverAccNo;
    float amount;
    char name[50];
    
    printf("\n");

    printf("         HELLO\n         WELCOME TO OUR BANK\n\n         Enter your account number: ");
    scanf("%d", &accNo);

    int foundAccount = checkAccount(filename, accNo, name);
    if (foundAccount) {
        printf("\n");
        printf("         Enter your PIN: ");
        scanf("%d", &pin);

        if (authenticateAccount(filename, accNo, pin, name)) {
            convertToUpperCase(name);
            printf("\n");
            printf("          HELLO %s\n", name);

            do {
                printf("\n          1. Withdraw Money\n          2. Balance Enquiry\n          3. Transfer Money\n          0. Exit\n");
                printf("\n          Choose an option: ");
                scanf("%d", &option);

                switch (option) {
                    case 1:
                        printf("\n");
                        printf("          Enter the amount to withdraw: ");
                        scanf("%f", &amount);
                        withdrawMoney(filename, accNo, pin, amount);
                        break;
                    case 2:
                        displayAccountBalance(filename, accNo, name);
                        break;
                    case 3:
                        printf("\n");
                        printf("          Enter the recipient's account number: ");
                        scanf("%d", &receiverAccNo);
                        printf("\n");
                        printf("          Enter the amount to transfer: ");
                        scanf("%f", &amount);
                        transferMoney(filename, accNo, pin, receiverAccNo, amount);
                        break;
                    case 0:
                        printf("          Exiting...\n");
                        break;
                    default:
                        printf("\n");
                        printf("          Invalid option. Please try again.\n");
                }
            } while (option != 0);
        } else {
            printf("Invalid PIN.\n");
        }
    } else {
        printf("Account not found.\n");
    }

    return 0;
}
