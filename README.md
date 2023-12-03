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
1.

int acccheck(char* filename, int acc_no) {
FILE* file = fopen(filename, "r");
    
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
}
    
int pincheck(char* filename, int acc_no, int pin, char* name) {
    FILE* file = fopen(filename, "r");

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
}
