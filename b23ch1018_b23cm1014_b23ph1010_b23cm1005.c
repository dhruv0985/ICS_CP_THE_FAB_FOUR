#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define red     "\x1b[31m" //give red colour
#define cyan   "\x1b[96m"//cyan
#define yellow  "\x1b[93m"//yellow
#define stripred    "\x1b[101m"//striped
#define reset   "\x1b[0m"//reset
#define bold   "\033[1;31m"//bold
#define nobold "\033[0m"

int acccheck(char* filename, int acc_no) {
    FILE* file = fopen(filename, "r");

    int fileaccno;
    while (fscanf(file, "%d,%*[^\n]", &fileaccno) ==1) {
        if (fileaccno == acc_no) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int pincheck(char* filename, int acc_no, int pin, char* name) {
    FILE* file = fopen(filename, "r");

    int fileaccno, filePin;
    float balance;
    while (fscanf(file, "%d,%d,%f,%[^\n]", &fileaccno, &filePin,&balance,name) == 4) {
        if (fileaccno == acc_no && filePin == pin) {
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

        int fileaccno, filePin;
        while (fscanf(file, "%d,%d,%f,%[^\n]\n", &fileaccno, &filePin, &balance, name) == 4) {
            if (fileaccno == acc_no) {
                if (balance >= amount) {
                    char s[] = ".txt";
                    char tdetail[100];
                    strcpy(tdetail, name);
                    strcat(tdetail, s);

                    FILE* tranfile=fopen(tdetail,"a+");
                    fprintf(tranfile,"       #  %.2fRs get debited successfully\n          Available balance %.2f\n",amount,(balance-amount));
                    fclose(tranfile);

                    balance -= amount;
                    printf(reset "\n          Withdrawal successful.\n          Remaining balance: %.2f\n", balance);
                } else {
                    printf(yellow"Insufficient funds. Withdrawal failed.\n");
                }
            }
            fprintf(tempFile, "%d,%d,%.2f,%s\n", fileaccno, filePin, balance, name);
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

        int fileaccno, filePin;
        while (fscanf(file, "%d,%d,%f,%[^\n]\n", &fileaccno, &filePin, &balance, name) == 4) {
            if (fileaccno == acc_no) {
                printf(cyan"\n          Name: %s\n", name);
                printf("\n          Account number: %d\n",fileaccno);
                printf("\n          Account balance: %.2f\n", balance);
                fclose(file);
                return;
            }
        }

        fclose(file);
    }
}


void changePIN(char* filename, int acc_no, int oldPIN) {
FILE* file = fopen(filename, "r");

FILE* tempFile = fopen("temp.txt", "w");

int newpin;
int fileaccno, filePIN;
float balance;
char name[50];
int updatedPIN = 0;

while (fscanf(file, "%d,%d,%f,%[^\n]", &fileaccno, &filePIN, &balance, name) == 4) {
    if (fileaccno == acc_no && filePIN == oldPIN) {
        printf(yellow"\n          Enter new pin :");
        printf("\033[1;31m");
        scanf("%d",&newpin);
        fprintf(tempFile, "%d,%d,%.2f,%s\n", fileaccno, newpin, balance, name);
        printf(reset"\n          ");
        printf(stripred" PIN change successful.");
        printf("\033[0m");
        printf("\n");
    }
    else if (fileaccno == acc_no && filePIN != oldPIN) {
        printf(red"          WRONG PIN"reset);
        fprintf(tempFile, "%d,%d,%.2f,%s\n", fileaccno, filePIN, balance, name);
    }
    else {
        fprintf(tempFile, "%d,%d,%.2f,%s\n", fileaccno, filePIN, balance, name);
    }
}

fclose(file);
fclose(tempFile);

remove(filename);

rename("temp.txt", filename);
}

void Transaction(char* filename, int acc_no, char* name) {
char s[] = ".txt";
char tdetail[100];
strcpy(tdetail, name);
strcat(tdetail, s);

FILE* file = fopen(tdetail, "r");

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

    int fileaccno, filePin;
    while (fscanf(file, "%d,%d,%f,%[^\n]", &fileaccno, &filePin, &balance, name) == 4) {
        if (fileaccno == acc_no) {
            balance = balance+amount;
            printf(reset "\n          Deposit successful.\n          Updated balance: %.2f\n", balance);

            char s[] = ".txt";
            char tdetail[100];
            strcpy(tdetail, name);
            strcat(tdetail, s);

            FILE* tranfile=fopen(tdetail,"a");
                    fprintf(tranfile,"       #  %.2fRs get credited successfully\n          Available balance %.2f\n",amount,balance);
                    fclose(tranfile);
        }
        fprintf(tempFile, "%d,%d,%.2f,%s\n", fileaccno, filePin, balance, name);
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename("temp.txt", filename);
}


int main() {
char filename[] = "account_details.txt";

int acc_no, pin, option, receiveraccno, newpin;
float amount;
char name[50];

printf("\n");
printf(bold cyan"                   *HELLO SIR*\n\n          WELCOME TO 'THE FAB FOUR' BANK ATM\n\n");
printf(yellow"          Enter your account number: ");
printf("\033[1;31m");
scanf("%d", &acc_no);
int foundaccount = acccheck(filename, acc_no);

if (foundaccount==1) {
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
            // printf("          4. Transfer Money\n");
            printf("          4. Mini Statement\n");
            printf("          5. Deposit Money\n");
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

                    Transaction(filename,acc_no,name);
                }
                else if(option==5){
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
