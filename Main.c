#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//void function for depositing money 
void depositAndUpdateBalance(float deposit, int numaccount){
    // Asks user for deposit amount
    printf("\nHow much would you like to deposit?\n$");
    scanf("%f", &deposit);

    // Open the balance file for reading and writing
    FILE *balanceFile = fopen("Balance.txt", "r+");
    if (balanceFile == NULL) {
        printf("Error opening Balance.txt\n");
        return;
    }

    int accountNumber;
    float existingBalanceA;
    int foundAccountA = 0;
    FILE *tempFile = fopen("temp.txt", "w"); // Temporary file for writing 

    // Find the position of the account number in the file and update balances
    while (fscanf(balanceFile, "%d = $%f", &accountNumber, &existingBalanceA) != EOF) {
        if (accountNumber == numaccount) {
            foundAccountA = 1;
            existingBalanceA += deposit; // Update the existing balance
        }
        fprintf(tempFile, "%d = $%.2f\n", accountNumber, existingBalanceA); // Write to temporary file
    }

    fclose(balanceFile);
    fclose(tempFile);

    if (!foundAccountA) {
        printf("Account not found in Balance.txt\n");
        remove("temp.txt"); // Remove temporary file
        return;
    }

    // Replace original file with the temporary file
    remove("Balance.txt");
    rename("temp.txt", "Balance.txt");
    printf("Deposit successful.\n");
}

//void function for withdrawing money
void withdrawAndUpdateBalance(float withdrawal, int numaccount){
    // Asks user for withdrawal amount
    printf("\nHow much would you like to withdraw?\n$");
    scanf("%f", &withdrawal);

    // Open the balance file for reading and writing
    FILE *balanceFile = fopen("Balance.txt", "r+");
    if (balanceFile == NULL) {
        printf("Error opening Balance.txt\n");
        return;
    }

    int accountNumber;
    float existingBalanceA;
    int foundAccountA = 0;
    FILE *tempFile = fopen("temp.txt", "w"); // Temporary file for writing updated balances

    // Find the position of the account number in the file and update balances
    while (fscanf(balanceFile, "%d = $%f", &accountNumber, &existingBalanceA) != EOF) {
        if (accountNumber == numaccount) {
            foundAccountA = 1;
            // Check if sufficient balance is available for withdrawal
            if (existingBalanceA >= withdrawal) {
                existingBalanceA -= withdrawal; // Update the existing balance
            } else {
                printf("Insufficient funds\n");
                fclose(balanceFile);
                fclose(tempFile);
                remove("temp.txt"); // Remove temporary file
                return;
            }
        }
        fprintf(tempFile, "%d = $%.2f\n", accountNumber, existingBalanceA); // Write to temporary file
    }

    fclose(balanceFile);
    fclose(tempFile);

    if (!foundAccountA) {
        printf("Account not found in Balance.txt\n");
        remove("temp.txt"); // Remove temporary file
        return;
    }

    // Replace original file with the temporary file
    remove("Balance.txt");
    rename("temp.txt", "Balance.txt");

    printf("Withdrawal successful.\n");
}

//void function for displaying account balance
void displayBalance(int numaccount) {
    FILE *balanceFile = fopen("Balance.txt", "r");
    if (balanceFile == NULL) {
        printf("Error opening Balance.txt\n");
        return;
    }

    int accountNumber;
    float balance;
    int foundAccount = 0;

    // Find the balance associated with the provided account number
    while (fscanf(balanceFile, "%d = $%f", &accountNumber, &balance) != EOF) {
        if (accountNumber == numaccount) {
            foundAccount = 1;
            printf("Account Balance:$%.2f\n\n", balance);
            break;
        }
    }

    fclose(balanceFile);

    if (!foundAccount) {
        printf("Account not found");
    }
}

//void function for making a new account
void CreateNewAccount(char newname[50]){
 FILE *AccountFile = fopen("Accounts.txt", "r+");
    if (AccountFile == NULL) {
        printf("Error opening Balance.txt\n");
        return;
    }
    createaccountnumber:
int randomNumber = (rand() % 900000) + 100000;

    int accountnumber;
    while(fscanf(AccountFile, "%d = $%d", &accountnumber)){
        if(accountnumber == randomNumber){
            goto createaccountnumber;
        }
        else{
            printf("%d",randomNumber);
        }
    }

}

//main function 
int main(void) {
    char option;
    int numaccount;
    float deposit;
    float withdraw;
    char choice;
    char newname[50];


    Start:

    printf("Welcome to Bank System! What would you like to do?\nA.Login\nB.Signup\n");
    scanf("%c", &choice);
    if (choice == 'a' || choice == 'A') 
{
  goto Login;
}
if (choice == 'b' || choice == 'B') 
{
  goto Signup;
}


  //inquires users accountnumber
  Login:
    printf("\nHello, Enter your 6 digit account number: ");
    scanf("%d", &numaccount);
  //Checks for account file
    FILE *file = fopen("Accounts.txt", "r");
    if (file == NULL) {
        printf("Error opening accounts.\n");
        return 1;
    }

    int found = 0;
    int accountNum;
    char name[50];
//Grabs users account and users name
    while (fscanf(file, "%d %s", &accountNum, name) != EOF) {
        if (accountNum == numaccount) {
            found = 1;
            printf("\nWelcome %s!\n", name);
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Sorry, account not found.\n");
        goto Login;
    }


//gives user account options
  Options:
    printf("Please choose from the options below\n\nA.Deposit\nB.Withdraw\nC.Display Balance\nD.Logout\n\n");
    scanf(" %c", &option);

  if (option == 'a' || option == 'A') 
{
  depositAndUpdateBalance(deposit, numaccount);

}
  if (option == 'b' || option == 'B')
{
  withdrawAndUpdateBalance(withdraw, numaccount);

}
  if (option == 'c' || option == 'C')
{
  displayBalance(numaccount);
 
}
  if (option == 'd' || option == 'D') 
{
    printf("Goodbye!\n\n");
   goto Start;
}
  if (option != 'a'&&option != 'A'&&
      option != 'b'&&option != 'B'&&
      option != 'c'&&option != 'C'){ 
     printf("Sorry that wasnt one of the options provided\n\n");
     goto Options;}
  
Signup:
printf("\nEnter your First Name\n");
    scanf("%49s", newname);  



  return 0;
}
