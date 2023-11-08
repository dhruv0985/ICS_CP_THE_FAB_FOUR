#include <stdio.h>

struct bank {
    char name[100];
    int acc_no;
    int bal;
    int pin;
};

int main() {
    struct bank customer[100];
    int acc = 10000;
    int coustno = 0;
    int input;

    printf("For new account, press 1\n");
    printf("For an already registered account, press 2\n");
    
    scanf("%d", &input);

    if (input == 1) {
        if (coustno < 100) { 
            printf("Enter your name: ");
            scanf("%s", customer[coustno].name);

            customer[coustno].acc_no = acc + coustno;
            printf("Your account number is: %d\n", customer[coustno].acc_no);

            printf("Enter your pin: ");
            scanf("%d", &customer[coustno].pin);

            customer[coustno].bal = 10000;
            coustno++;
        } else {
            printf("Bank is at full capacity. Cannot create a new account.\n");
        }
    }

    return 0;
}

