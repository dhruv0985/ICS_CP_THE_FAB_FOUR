//create a file name "account_details.txt" and save person account details in the sequence{account no,pin,amount,name}
//remember don't give space between them
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
//these are code that give colour
#define red     "\x1b[31m"//give red colour
#define cyan   "\x1b[96m"//cyan
#define yellow  "\x1b[93m"//yellow
#define stripred    "\x1b[101m"//striped
#define reset   "\x1b[0m"//reset
#define bold   "\033[1;31m"//bold
#define nobold "\033[0m"

int acccheck(char* filename, int acc_no) {
    FILE* file = fopen(filename, "r");

    int fileAccNo;
    while (fscanf(file, "%d,%*[^\n]", &fileAccNo) ==1) {
        if (fileAccNo == acc_no) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int pincheck(char* filename, int acc_no, int pin, char* name) {
    FILE* file = fopen(filename, "r");

    int fileAccNo, filePin;
    float balance;
    while (fscanf(file, "%d,%d,%f,%[^\n]", &fileAccNo, &filePin,&balance,name) == 4) {
        if (fileAccNo == acc_no && filePin == pin) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void withdrawmoney(char* filename, int acc_no, int pin, float amount) {
    float balance;
    char name[50];


        FILE* file = fopen(filename, "r");

        FILE* tempFile = fopen("temp.txt", "w");

        int fileAccNo, filePin;
        while (fscanf(file, "%d,%d,%f,%[^\n]\n", &fileAccNo, &filePin, &balance, name) == 4) {
            if (fileAccNo == acc_no) {
                if (balance >= amount) {
                    char s[] = ".txt";
                    char tran_details[100];
                    strcpy(tran_details, name);
                    strcat(tran_details, s);

                    FILE* tranfile=fopen(tran_details,"a+");
                    fprintf(tranfile,"       #  %.2fRs get debited successfully\n          Available balance %.2f\n",amount,(balance-amount));
                    fclose(tranfile);

                    balance -= amount;
                    printf(reset "\n          Withdrawal successful.\n          Remaining balance: %.2f\n", balance);
                } else {
                    printf(yellow"Insufficient funds. Withdrawal failed.\n");
                }
            }
            fprintf(tempFile, "%d,%d,%.2f,%s\n", fileAccNo, filePin, balance, name);
        }

        fclose(file);
        fclose(tempFile);

        remove(filename);
        rename("temp.txt",filename);
}

void printbalance(char* filename, int acc_no, char* name) {
    float balance;

    if (acccheck(filename, acc_no)==1) {
        FILE* file = fopen(filename, "r");

        int fileAccNo, filePin;
        while (fscanf(file, "%d,%d,%f,%[^\n]\n", &fileAccNo, &filePin, &balance, name) == 4) {
            if (fileAccNo == acc_no) {
                printf(cyan"\n          Name: %s\n", name);
                printf("\n          Account number: %d\n",fileAccNo);
                printf("\n          Account balance: %.2f\n", balance);
                fclose(file);
                return;
            }
        }

        fclose(file);
    }
}

void transfermoney(char* filename, int senderAccNo, int senderPin, int receiverAccNo) {
    FILE* file = fopen(filename, "r");

    int senderFound = 0;
    int receiverFound = 0;
    int sendernum;
    int receivernum;

    struct Account {
        int acc_no;
        int pin;
        float balance;
        char name[50];
    };

    struct Account accounts[100];
    int numAccounts = 0;

    while (fscanf(file, "%d,%d,%f,%[^\n]\n", &accounts[numAccounts].acc_no, &accounts[numAccounts].pin, &accounts[numAccounts].balance, accounts[numAccounts].name) == 4) {
        if (accounts[numAccounts].acc_no == senderAccNo) {
            senderFound = 1;
            sendernum = numAccounts;
        }

        if (accounts[numAccounts].acc_no == receiverAccNo) {
            receivernum = numAccounts;
            printf(yellow"          This account belongs to ");
            printf(cyan"%s\n\n", accounts[numAccounts].name);
            receiverFound = 1;
        }

        numAccounts++;
    }

    if (receiverFound == 1 && senderFound == 1) {
        float amount;

        printf(yellow"          Enter the amount to transfer: ");
        printf("\033[1;31m");
        scanf("%f", &amount);

        if (accounts[sendernum].balance >= amount) {

            char newfile[] = ".txt";
            char sender_details[100];
            char receiver_details[100];

            strcpy(sender_details, accounts[sendernum].name);
            strcat(sender_details, newfile);

            strcpy(receiver_details, accounts[receivernum].name);
            strcat(receiver_details, newfile);

            FILE *senderFile = fopen(sender_details, "a");
            FILE *receiverFile = fopen(receiver_details, "a");

            accounts[sendernum].balance -= amount;
            accounts[receivernum].balance += amount;

            printf(reset"\n\n          Transfer successful from %s's account.\n          Remaining balance: %.2f\n", accounts[sendernum].name, accounts[sendernum].balance);
            printf("\n\n          Transfer successful to %s's account.\n", accounts[receivernum].name);
            fprintf(senderFile, "       #  Transfer successful of amount : %.2fRs to account number : %d\n          Available balance : %.2f\n", amount, accounts[receivernum].acc_no, accounts[sendernum].balance);
            fclose(senderFile);
            fprintf(receiverFile, "       #  %.2fRs amount get credited from account number :%d\n          Updated balance is %.2f\n", amount, accounts[sendernum].acc_no, accounts[receivernum].balance);
            fclose(receiverFile);
        } else {
            printf(yellow"          Insufficient funds. Transfer failed.\n");
            fclose(file);
            return;
        }
    }

    fclose(file);


    if (!receiverFound) {
        printf(yellow"          Receiver account not found.\n\n");
        return;
    }

    file = fopen(filename, "w");

    for (int i = 0; i < numAccounts; i++) {
        fprintf(file, "%d,%d,%.2f,%s\n", accounts[i].acc_no, accounts[i].pin, accounts[i].balance, accounts[i].name);
    }

    fclose(file);
}


void changePIN(char* filename, int acc_no, int oldPIN) {
FILE* file = fopen(filename, "r");

FILE* tempFile = fopen("temp.txt", "w");

int newpin;
int fileAccNo, filePIN;
float balance;
char name[50];
int updatedPIN = 0;

while (fscanf(file, "%d,%d,%f,%49[^\n]\n", &fileAccNo, &filePIN, &balance, name) == 4) {
    if (fileAccNo == acc_no && filePIN == oldPIN) {
        printf(yellow"\n          Enter new pin :");
        printf("\033[1;31m");
        scanf("%d",&newpin);
        fprintf(tempFile, "%d,%d,%.2f,%s\n", fileAccNo, newpin, balance, name);
        printf(reset"\n          ");
        printf(stripred" PIN change successful.");
        printf("\033[0m");
        printf("\n");
    }
    else if (fileAccNo == acc_no && filePIN != oldPIN) {
        printf(red"          WRONG PIN"reset);
        fprintf(tempFile, "%d,%d,%.2f,%s\n", fileAccNo, filePIN, balance, name);
    }
    else {
        fprintf(tempFile, "%d,%d,%.2f,%s\n", fileAccNo, filePIN, balance, name);
    }
}

fclose(file);
fclose(tempFile);

remove(filename);

rename("temp.txt", filename);
}

void Transaction(char* filename, int acc_no, char* name) {
char s[] = ".txt";
char tran_details[100];
strcpy(tran_details, name);
strcat(tran_details, s);

FILE* file = fopen(tran_details, "r");

printf("\n\n");
char ch;
while ((ch = fgetc(file)) != EOF) {
    printf(reset "%c",ch);
}

fclose(file);
}

void depositmoney(char* filename, int acc_no, float amount, char* name) {
    float balance;

    FILE* file = fopen(filename, "r");
    FILE* tempFile = fopen("temp.txt", "w");

    int fileAccNo, filePin;
    while (fscanf(file, "%d,%d,%f,%[^\n]", &fileAccNo, &filePin, &balance, name) == 4) {
        if (fileAccNo == acc_no) {
            balance = balance+amount;
            printf(reset "\n          Deposit successful.\n          Updated balance: %.2f\n", balance);

            char s[] = ".txt";
            char tran_details[100];
            strcpy(tran_details, name);
            strcat(tran_details, s);

            FILE* tranfile=fopen(tran_details,"a");
                    fprintf(tranfile,"       #  %.2fRs get credited successfully\n          Available balance %.2f\n",amount,balance);
                    fclose(tranfile);
        }
        fprintf(tempFile, "%d,%d,%.2f,%s\n", fileAccNo, filePin, balance, name);
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename("temp.txt", filename);
}


int main() {
char filename[] = "account_details.txt";

int acc_no, pin, option, receiverAccNo, newPIN;
float amount;
char name[50];

printf("\n");
printf(bold cyan"                   *HELLO SIR*\n\n          WELCOME TO 'THE FAB FOUR' BANK ATM\n\n");
printf(yellow"          Enter your account number: ");
printf("\033[1;31m");
scanf("%d", &acc_no);
int foundAccount = acccheck(filename, acc_no);

if (foundAccount==1) {
    printf("\n");
    printf(yellow"          Enter your PIN: ");
    printf("\033[1;31m");
    scanf("%d", &pin);
    printf("\033[0m");
    if (pincheck(filename, acc_no, pin, name)==1) {
        printf("\n");
        printf(bold yellow"                  HELLO ");
        printf(cyan"%s\n\n",name);

        do{
            printf(bold yellow"\n          1. View Account Details\n");
            printf("          2. Change Pin\n");
            printf("          3. Withdraw Money\n");
            printf("          4. Transfer Money\n");
            printf("          5. Mini Statement\n");
            printf("          6. Deposit Money\n");
            printf("          0. Exit\n");

            printf("\n          Choose an option: ");
            printf("\033[1;31m");
            scanf("%d", &option);

                if(option==1){
                    printbalance(filename, acc_no, name);
                }
                else if(option==2){
                    printf("\n");
                    printf(yellow"          Enter your old PIN: ");
                    printf("\033[1;31m");
                    scanf("%d", &pin);
                    changePIN(filename, acc_no, pin);
                    }  
                else if(option==3){
                    printf("\n");
                    printf(yellow"          Enter the amount to withdraw: ");
                    printf("\033[1;31m");
                    scanf("%f", &amount);
                    withdrawmoney(filename, acc_no, pin, amount);
                }
                else if(option==4){
                    printf("\n");
                    printf(yellow"          Enter the recipient's account number: ");
                    printf("\033[1;31m");
                    scanf("%d", &receiverAccNo);
                    printf("\n");
                    transfermoney(filename, acc_no, pin, receiverAccNo);
                }
                else if(option==5){

                    Transaction(filename,acc_no,name);
                }
                else if(option==6){
                    printf(yellow"\n          Enter The Amount to Deposit :");
                    printf("\033[1;31m");
                    scanf("%f",&amount);
                    printf("\033[0m");
                    depositmoney(filename, acc_no, amount,name);
                }
                else if(option==0){
                    printf("         ");
                    printf(stripred cyan" Exiting...");
                    printf("\033[0m");
                    printf("\n");
                    break;
                }
                else{
                    printf("\n");
                    printf(yellow"          Invalid option\n          Choose one of the above 6 options.\n");
                }
        }
        while(0<=option);
    }
    else {
        printf("\033[1;31m");
        printf("\n         ");
        printf(stripred cyan" **Invalid PIN.**");
        printf("\033[0m");
        printf("\n");
    }
} else {
    printf("\n          ");
    printf(stripred cyan" **Account not found.**");
    printf("\033[0m");
}

return 0;
}
