TEAM MEMBER:-
1. HEMANT PATEL B23CH1018
2. DHRUV KUMAR TIWARI B23CM1014
3. AKANKSHA SINGH
4. KRITI BANSAL

ENVIROMENT SETUP

a) CODE DEPENDENCIES

1.ACCOUNT DETAILS:- https://github.com/dhruv0985/ICS_CP_THE_FAB_FOUR/blob/77db3b91625b9a8d5c2a6c787f345d5952a9c836/account_details.txt

EXECUTION GUIDELINES
1. enter account no of 8 digits
2. enter 4 digit pin
3. then chosse option

WORKING OF CODE

We use simple name variable so it is easy to understand what that variable do and, function do in our code.

1.This part check that our input accountno. and pin is present in code or not  
    
    int fileAccNo;
    while (fscanf(file, "%d,%*[^,\n],%*[^\n]", &fileAccNo) ==1) {
        if (fileAccNo == acc_no) {
            fclose(file);
            return 1;
        }
        fseek(file, 1, SEEK_CUR);
    }

    fclose(file);
    return 0; 

    int fileAccNo, filePin;
    float balance;
    while (fscanf(file, "%d,%d,%f,%[^\n]", &fileAccNo, &filePin, &balance, name) == 4) {
        if (fileAccNo == acc_no && filePin == pin) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;

This part transfer money from one account to other and also save history in another file

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
            
            char extension[] = ".txt";
            char sender_details[100];
            char receiver_details[100];

            strcpy(sender_details, accounts[sendernum].name);
            strcat(sender_details, extension);

            strcpy(receiver_details, accounts[receivernum].name);
            strcat(receiver_details, extension);

            FILE *senderFile = fopen(sender_details, "a+");
            FILE *receiverFile = fopen(receiver_details, "a+");

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

    if (!senderFound) {
        printf(yellow"          Sender account not found.\n\n");
        return;
    }

    if (!receiverFound) {
        printf(yellow"          Receiver account not found.\n\n");
        return;
    }

    file = fopen(filename, "w");

    for (int i = 0; i < numAccounts; i++) {
        fprintf(file, "%d,%d,%.2f,%s\n", accounts[i].acc_no, accounts[i].pin, accounts[i].balance, accounts[i].name);
    }

    fclose(file);

Similarly withdrawmoney function ,depositmoney function ,transaction history are also coded.
