#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include<string.h>

void convertToUpperCase(char* str) {
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

int checkAccount(char* filename, int accNo) {
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
    while (fscanf(file, "%d,%*[^,\n],%*[^\n]", &fileAccNo) ==1) {
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

    
        FILE* file = fopen(filename, "r+");

        FILE* tempFile = fopen("temp.txt", "w");

        int fileAccNo, filePin;
        while (fscanf(file, "%d,%d,%f,%[^\n]\n", &fileAccNo, &filePin, &balance, name) == 4) {
            if (fileAccNo == accNo) {
                if (balance >= amount) {
                    char s[] = ".txt";
                    char tran_details[100];
                    strcpy(tran_details, name);
                    strcat(tran_details, s);
                    
                    FILE* tranfile=fopen(tran_details,"a+");
                    fprintf(tranfile,"          %.2fRs get debited successfully\n          Available balance %.2f\n",amount,balance);
                    fclose(tranfile);
                    
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
    
}

// Function to display account balance
void displayAccountBalance(char* filename, int accNo, char* name) {
    float balance;

    if (checkAccount(filename, accNo)) {
        FILE* file = fopen(filename, "r");

        int fileAccNo, filePin;
        while (fscanf(file, "%d,%d,%f,%[^\n]\n", &fileAccNo, &filePin, &balance, name) == 4) {
            if (fileAccNo == accNo) {
                printf("\n          Name: %s\n", name);
                printf("\n          Account number: %d\n",fileAccNo);
                printf("\n          Account balance: %.2f\n", balance);
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

void transferMoney(char* filename, int senderAccNo, int senderPin, int receiverAccNo) {
    FILE* file = fopen(filename, "r");

    int senderFound = 0;
    int receiverFound = 0;
    int sendernum;
    int receivernum;

    struct Account {
        int accNo;
        int pin;
        float balance;
        char name[50];
    };

    struct Account accounts[100];  // Assuming a maximum of 100 accounts
    int numAccounts = 0;

    while (fscanf(file, "%d,%d,%f,%[^\n]\n", &accounts[numAccounts].accNo, &accounts[numAccounts].pin, &accounts[numAccounts].balance, accounts[numAccounts].name) == 4) {
        
    
        if (accounts[numAccounts].accNo == senderAccNo) {
            senderFound = 1;
            sendernum=numAccounts;
            
        }

        if (accounts[numAccounts].accNo == receiverAccNo) {
            receivernum=numAccounts;
            // accounts[numAccounts].balance += amount;
            receiverFound = 1;
        }
        

        numAccounts++;
    }
    
    if(receiverFound==1 && senderFound==1){
        float amount;
        printf("          Enter the amount to transfer: ");
        scanf("%f", &amount);
        if (accounts[sendernum].balance >= amount) {
            
            char s[] = ".txt";
            char tran_details_sender[100];
            char tran_details_receiver[100];

            strcpy(tran_details_sender, accounts[sendernum].name);
            strcat(tran_details_sender, s);

            strcpy(tran_details_receiver, accounts[receivernum].name);
            strcat(tran_details_receiver, s);

            FILE *coustdetails = fopen(tran_details_sender, "a+");
            FILE *recdetails = fopen(tran_details_receiver, "a+");

                accounts[sendernum].balance -= amount;
                accounts[receivernum].balance += amount;
                printf("\n\n          Transfer successful from %s's account.\n          Remaining balance: %.2f\n", accounts[sendernum].name, accounts[sendernum].balance);
                printf("\n\n          Transfer successful to %s's account.",accounts[receivernum].name);
                fprintf(coustdetails,"          Transfer successful of amount : %.2fRs to account number : %d\n          Available balance : %.2f\n",amount,accounts[receivernum].accNo,accounts[sendernum].balance);
                fclose(coustdetails);
                fprintf(recdetails,"          %.2fRs amount get credited from account number :%d\n          Updated balance is %.2f\n",amount,accounts[sendernum].accNo,accounts[receivernum].balance);
        } else {
            printf("          Insufficient funds. Transfer failed.\n");
            fclose(file);
            return;
        }
    }
    
   
    fclose(file);

    if (!senderFound) {
        printf("          Sender account not found.\n\n");
        return;
    }

    if (!receiverFound) {
        printf("          Receiver account not found.\n\n");
        return;
    }

    file = fopen(filename, "w");

    for (int i = 0; i < numAccounts; i++) {
        fprintf(file, "%d,%d,%.2f,%s\n", accounts[i].accNo, accounts[i].pin, accounts[i].balance, accounts[i].name);
    }

    fclose(file);
}

void changePIN(char* filename, int accNo, int oldPIN, int newPIN) {
    FILE* file = fopen(filename, "r+");
    
    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        fclose(file);
        printf("Error creating temporary file.\n");
        return;
    }

    int fileAccNo, filePIN;
    float balance;
    char name[50];
    int updatedPIN = 0;

    while (fscanf(file, "%d,%d,%f,%49[^\n]\n", &fileAccNo, &filePIN, &balance, name) == 4) {
        if (fileAccNo == accNo && filePIN == oldPIN) {
            fprintf(tempFile, "%d,%d,%.2f,%s\n", fileAccNo, newPIN, balance, name);
            printf("\n          PIN change successful.\n");
            updatedPIN = 1;
        } else {
            fprintf(tempFile, "%d,%d,%.2f,%s\n", fileAccNo, filePIN, balance, name);
        }
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

    if (!updatedPIN) {
        printf("Account not found or incorrect old PIN.\n");
    }
}
void Transaction(char* filename, int accNo, char* name) {
  char s[] = ".txt";
  char tran_details[100];
  strcpy(tran_details, name);
  strcat(tran_details, s);

  FILE* file = fopen(tran_details, "r");
  if (file == NULL) {
    printf("Error opening file for reading.\n");
    return;
  }
printf("\n\n");
  char ch;
  while ((ch = fgetc(file)) != EOF) {
    putchar(ch);
  }

  fclose(file);
}


int main() {
    char filename[] = "account_details.txt";

    int accNo, pin, option, receiverAccNo, newPIN;
    float amount;
    char name[50];

    printf("\n");
    printf("         **HELLO SIR**\n         WELCOME TO OUR BANK\n\n         Enter your account number: ");
    scanf("%d", &accNo);

    int foundAccount = checkAccount(filename, accNo);
    if (foundAccount==1) {
        printf("\n");
        printf("         Enter your PIN: ");
        scanf("%d", &pin);

        if (authenticateAccount(filename, accNo, pin, name)) {
            convertToUpperCase(name);
            printf("\n");
            printf("          HELLO %s\n", name);

            do {
                printf("\n\n          1. Withdraw Money\n          2. Balance Enquiry\n          3. Transfer Money\n          4. Change PIN\n          5. Transaction History\n          0. Exit\n");
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
                        transferMoney(filename, accNo, pin, receiverAccNo);
                        break;
                    case 4:
                        printf("\n");
                        printf("          Enter your old PIN: ");
                        scanf("%d", &pin);
                        printf("\n          Enter your new PIN: ");
                        scanf("%d", &newPIN);
                        changePIN(filename, accNo, pin, newPIN);
                        break;
                    case 5:
                    
                        Transaction(filename,accNo,name);
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
            printf("\n          Invalid PIN.\n");
        }
    } else {
        printf("\n           Account not found.\n");
    }

    return 0;
}
