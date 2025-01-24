#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

static int balance;
char transactionHistory[1000] = "";

void saveTransaction(const char *transaction) {
    strcat(transactionHistory, transaction);
    strcat(transactionHistory, "\n");
}

void withdrawMoney() {
    int amountToWithdraw;
    printf("\033[1;33mEnter the amount you want to withdraw..: \033[0m");
    scanf("%d", &amountToWithdraw);
    if (amountToWithdraw > balance) {
        printf("\033[1;31mInsufficient balance.\033[0m\n");
    } else {
        balance -= amountToWithdraw;
        printf("\033[1;32mWithdrawal completed successfully.\033[0m\n");
        char transaction[50];
        sprintf(transaction, "Withdrawn: %d TL", amountToWithdraw);
        saveTransaction(transaction);
    }
    sleep(2);
}

void depositMoney() {
    int amountToDeposit;
    printf("\033[1;36mEnter the amount you want to deposit..: \033[0m");
    scanf("%d", &amountToDeposit);
    balance += amountToDeposit;
    printf("\033[1;32mDeposit completed successfully.\033[0m\n");
    char transaction[50];
    sprintf(transaction, "Deposited: %d TL", amountToDeposit);
    saveTransaction(transaction);
    sleep(2);
}

void payBill() {
    int choice, billAmount;
    char *billType[] = {"Electricity", "Natural Gas", "Water", "Phone"};
    billAmount = rand() % 100 + 20;

    printf("\033[1;35m1-->Electricity\n2-->Natural Gas\n3-->Water\n4-->Phone\033[0m\n");
    printf("\033[1;33mSelect your transaction..: \033[0m");
    scanf("%d", &choice);

    if (choice >= 1 && choice <= 4) {
        printf("\033[1;36mYour %s bill: %d TL\033[0m\n", billType[choice - 1], billAmount);
        printf("\033[1;33mDo you want to pay? (Y-->1 / N-->2): \033[0m");
        int response;
        scanf("%d", &response);

        if (response == 1) {
            if (billAmount > balance) {
                printf("\033[1;31mInsufficient balance.\033[0m\n");
            } else {
                balance -= billAmount;
                printf("\033[1;32mBill payment completed successfully.\033[0m\n");
                char transaction[50];
                sprintf(transaction, "%s bill paid: %d TL", billType[choice - 1], billAmount);
                saveTransaction(transaction);
            }
        } else {
            printf("\033[1;33mTransaction canceled.\033[0m\n");
        }
    } else {
        printf("\033[1;31mInvalid choice.\033[0m\n");
    }
    sleep(2);
}

void transferMoney() {
    int iban = 101010, ibanNo, amount;
    printf("\033[1;36mEnter IBAN number..: \033[0m");
    scanf("%d", &ibanNo);
    if (ibanNo == iban) {
        printf("\033[1;33mEnter the amount you want to send..: \033[0m");
        scanf("%d", &amount);
        if (amount > balance) {
            printf("\033[1;31mInsufficient balance.\033[0m\n");
        } else {
            balance -= amount;
            printf("\033[1;32mTransfer completed successfully.\033[0m\n");
            char transaction[50];
            sprintf(transaction, "Transferred: %d TL", amount);
            saveTransaction(transaction);
        }
    } else {
        printf("\033[1;31mInvalid IBAN entered.\033[0m\n");
    }
    sleep(2);
}

void showTransactionHistory() {
    printf("\n\033[1;34m--- Transaction History ---\033[0m\n");
    printf("\033[1;37m%s\033[0m\n", transactionHistory);
    printf("\033[1;34m---------------------------\033[0m\n");
    sleep(2);
}

int main() {
    system("color 02");
    srand(time(NULL));
    balance = rand() % 1000 + 100;
    int password = 123123, passwordInput, attemptsRemaining = 3, choice;
    char username[50];

    printf("\033[1;33mPlease enter your username: \033[0m");
    scanf("%s", username);

    while (attemptsRemaining > 0) {
        printf("\n\033[1;33mPlease enter your password: \033[0m");
        scanf("%d", &passwordInput);

        if (passwordInput == password) {
            printf("\033[1;32mLogin successful. Welcome, %s!\033[0m\n", username);
            break;
        } else {
            attemptsRemaining--;
            printf("\033[1;31mLogin failed. Remaining attempts: %d\033[0m\n", attemptsRemaining);
            if (attemptsRemaining == 0) {
                printf("\033[1;31mYour card has been blocked. Please contact customer service.\033[0m\n");
                exit(0);
            }
        }
    }

    while (1) {
        system("cls");
        printf("\033[1;35m%67s\033[0m", "Welcome to Ferhan ATM\n\n");
        printf("\033[1;31m%75s\033[0m", "*****************************************\n");
        printf("\033[1;32m%75s\033[0m", "*****      1--->Withdraw            *****\n");
        printf("\033[1;32m%75s\033[0m", "*****      2--->Deposit             *****\n");
        printf("\033[1;32m%75s\033[0m", "*****      3--->Pay Bill            *****\n");
        printf("\033[1;32m%75s\033[0m", "*****      4--->Transfer            *****\n");
        printf("\033[1;32m%75s\033[0m", "*****      5--->Transaction History *****\n");
        printf("\033[1;32m%75s\033[0m", "*****      6--->Exit                *****\n");
        printf("\033[1;31m%75s\033[0m", "*****************************************\n");
        printf("\033[1;34m%59s%d\033[0m\n\n", "Your Balance: ", balance);

        printf("\033[1;33m\nSelect your transaction..: \033[0m");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                withdrawMoney();
                break;
            case 2:
                depositMoney();
                break;
            case 3:
                payBill();
                break;
            case 4:
                transferMoney();
                break;
            case 5:
                showTransactionHistory();
                break;
            case 6:
                printf("\033[1;32mExiting. Thank you, %s!\033[0m\n", username);
                exit(0);
            default:
                printf("\033[1;31mInvalid choice. Please try again.\033[0m\n");
                sleep(2);
        }
    }
    return 0;
}

