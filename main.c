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
                    printf("Withdrawal successful. Remaining balance: %.2f\n", balance);
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

int main() {
    char filename[] = "account_details.txt";

    int accNo, pin, option;
    float amount;
    char name[50];

    printf("Enter your account number: ");
    scanf("%d", &accNo);

    int foundAccount = checkAccount(filename, accNo, name);
    if (foundAccount) {
        printf("Enter your PIN: ");
        scanf("%d", &pin);

        if (authenticateAccount(filename, accNo, pin, name)) {
            convertToUpperCase(name);
            printf("HELLO %s\n", name);

            do {
                printf("\n1. Withdraw Money\n2. Balance Enquiry\n0. Exit\n");
                printf("Choose an option: ");
                scanf("%d", &option);

                switch (option) {
                    case 1:
                        printf("Enter the amount to withdraw: ");
                        scanf("%f", &amount);
                        withdrawMoney(filename, accNo, pin, amount);
                        break;
                    case 2:
                        displayAccountBalance(filename, accNo, name);
                        break;
                    case 0:
                        printf("Exiting...\n");
                        break;
                    default:
                        printf("Invalid option. Please try again.\n");
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
