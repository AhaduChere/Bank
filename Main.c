#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

//set account number length
#define ACCOUNT_LENGTH 6 

//clears screen so it looks nice in terminal
void clearScreen() {
    printf("\033[H\033[J"); 

}

//removes previous inputs from past menus
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != EOF && c != '\n');
}

//void function for depositing money 
void depositAndUpdateBalance(float deposit, int numaccount){
    // Asks user for deposit amount
    printf("\nHow much would you like to deposit?($0 if none)\n$");
    scanf("%f", &deposit);

    // Open the balance file for reading and writing
    FILE *balanceFile = fopen("Balance.txt", "r");
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
        fprintf(tempFile, "\n%d = $%.2f", accountNumber, existingBalanceA); // Write new balance to temporary file
    }

    //close files
    fclose(balanceFile);
    fclose(tempFile);

    if (!foundAccountA) {
        printf("Account not found in Balance.txt\n");
        remove("temp.txt"); // Remove temporary file
        return;
    }

    // Replace original file with the temporary file done
    remove("Balance.txt");
    rename("temp.txt", "Balance.txt");
    clearScreen();          
    if (deposit > 0){
    printf("Deposit successful.\n\n");
    }
}

//void function for withdrawing money
void withdrawAndUpdateBalance(float withdrawal, int numaccount){
    // Asks user for withdrawal amount
    printf("\nHow much would you like to withdraw?($0 if none)\n$");
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
                existingBalanceA -= withdrawal; // Update the existing balance if funds available
            } else {
                printf("Insufficient funds\n");
                fclose(balanceFile);//Closes balance file
                fclose(tempFile);
                remove("temp.txt"); // Remove temporary file
                return;
            }
        }
        fprintf(tempFile, "\n%d = $%.2f", accountNumber, existingBalanceA); // Write new balance to temporary file
    }

    //close files
    fclose(balanceFile);
    fclose(tempFile);

    if (!foundAccountA) {
        printf("Account not found in Balance.txt\n");
        remove("temp.txt"); // Remove temporary file
        return;
    }

    // Replace original file with the temporary file and finishes
    remove("Balance.txt");
    rename("temp.txt", "Balance.txt");
    clearScreen();
   if (withdrawal > 0){
    printf("Withdrawal successful.\n\n");
    }
 }

//void function for displaying account balance
void displayBalance(int numaccount) {

    FILE *balanceFile = fopen("Balance.txt", "r");
    if (balanceFile == NULL) {
        printf("Error opening Balance.txt\n"); //check if balance file available
        return;
    }

    int accountNumber;
    float balance;
    int foundAccount = 0;

    // Find the balance associated with the provided account number and stores it
    while (fscanf(balanceFile, "%d = $%f", &accountNumber, &balance) != EOF) {
        if (accountNumber == numaccount) {
            foundAccount = 1;
            printf("Account Balance:$%.2f\n\n", balance); //prints account balance
            break;
        }
    }

    fclose(balanceFile); //closes file

    if (!foundAccount) {
        printf("Account not found"); //if users account doesnt exist
    }
}

//Void function for login
void Login(int numaccount, float deposit, float withdraw, char option, char leaveLog) {
    char input[10];
    int invalid = 0;  
    clearScreen();
    clearInputBuffer();
    Login:
    while (1) {
              
        // Display the message only if the loop is replayed meaning they entered invalid stuff
        if (invalid) {
            printf("Invalid entry. Please choose from the options given\n");
        }
        
        printf("Enter your 6-digit account number or Q to return: "); //asks for account number
        fgets(input, sizeof(input), stdin);

        size_t len = strlen(input);
        while (len > 0 && (input[len - 1] == '\n' || input[len - 1] == ' ')) {// Remove newline character or spaces at the end of input
        input[len - 1] = '\0';
        len--;
}

        if (input[0] == 'Q' || input[0] == 'q') { // check if user wants to go back
            printf("\033[H\033[J");
            return;
        }

        int all_digits = 1;  
        if (strlen(input) != ACCOUNT_LENGTH) {//check if 6 digits
            all_digits = 0;
        } else {
            for (int i = 0; i < ACCOUNT_LENGTH; i++) {//checks if all things entered are digits
                if (!isdigit(input[i])) {
                    all_digits = 0;
                    break;
                }
            }
        }

        
        if (all_digits) { //if the input is valid
            numaccount = atoi(input);
            invalid = 0;
            
        }
        if (!all_digits) { //if the input isnt valid
            invalid = 1;
            clearScreen();
            continue;
        }
        
     //Checks for account file
    FILE *file = fopen("Accounts.txt", "r");
    if (file == NULL) {
        printf("Error accessing accounts.\n");
        return;
    }

 int found = 0;
int accountNum;
char name[50];

while (fscanf(file, "%d %s", &accountNum, name) != EOF) { // Grabs users' account and users' name
    if (accountNum == numaccount) {
        found = 1;
        clearScreen();
        printf("Welcome %s!\n", name);
        fclose(file);
         
    }
}

fclose(file); // Close the file after processing is complete

if(found){ //exit loop
    break;
}
if (!found) { // If the account was not found reset values and begin loop at line 159 again
    clearScreen();
    printf("Sorry, account not found.\n");
    numaccount = -1;
}
           
}  
    Options:
    
  printf("Please choose from the options below\n\nA.Deposit\nB.Withdraw\nC.Display Balance\nD.Logout\n\n"); //choices after logging in
    scanf(" %c", &option);

  if (option == 'a' || option == 'A') 
{clearScreen();
  depositAndUpdateBalance(deposit, numaccount); //deposit
  goto Options;
}

  if (option == 'b' || option == 'B')
{clearScreen();
  withdrawAndUpdateBalance(withdraw, numaccount); // withdrawal
  goto Options;
}

  if (option == 'c' || option == 'C') //display balace
{clearScreen();
  displayBalance(numaccount);
  goto Options;
}

  if (option == 'd' || option == 'D') //Logout
{   
    numaccount = -1;
    clearScreen();
 }
                                      //if they enter something else
  if (option != 'a'&&option != 'A'&&    
      option != 'b'&&option != 'B'&&
      option != 'c'&&option != 'C'&&
      option != 'd'&&option != 'D'){ 
     printf("Sorry that wasnt one of the options provided\n\n");
     goto Options;}
       }

//void function for making a new account
void CreateNewAccount(char newname[50],int randNum,int accounts,char users[50]){
    clearInputBuffer();
    clearScreen();

  printf("Enter your First Name or Q to return:"); //Asks for name for account or can choose to go back
    scanf("%49s", newname); 
      if (strcmp(newname, "Q") == 0 || strcmp(newname, "q") == 0) {
        clearScreen();
        return; // Exit the function
    } 

 FILE *AccountFile = fopen("Accounts.txt", "r"); //opens file in read mode 
    if (AccountFile == NULL) {
        printf("Error opening Accounts.txt\n");
        return;
    } 
while (fscanf(AccountFile, "%d %s", &accounts, users) == 2) {//check if user already has an account
    if (strcasecmp(users, newname) == 0) {
        clearScreen();
        printf("You already have an account with us\n\n");
        fclose(AccountFile); 
        return;
    }
}
    fclose(AccountFile);
    createaccountnumber:
     AccountFile = fopen("Accounts.txt", "r+"); // opens file in read and write mode
    if (AccountFile == NULL) {
        printf("Error opening Accounts.txt\n");
        return;
    }
    generate:
    srand(time(NULL));
    randNum = rand() % 900000 + 100000;      //generates account number
    
    while(fscanf(AccountFile, "%d = %c", &accounts)){ // Read existing account numbers
        if(accounts == randNum){
            goto generate;     // Regenerate if the number already exists
        }
        clearScreen();
         printf("Your new account number is %d\n\n",randNum); //tells user new account number

            AccountFile = fopen("Accounts.txt", "a");
               
            fprintf(AccountFile, "\n%d %s", randNum, newname); //writes to account file
            fclose(AccountFile);
            
            FILE*BalanceFile = fopen("Balance.txt","a"); //writes to balance file
            fprintf(BalanceFile,"\n%d = $0.00");
            fclose(BalanceFile); 
            return;
            }
       }
    


//main function 
int main() {
    char option;
    int numaccount;
    float deposit;
    float withdraw;
    char choice;
    char newname[50];
    int randNum;
    int accounts;
    char users[50];
    char leaveLog;
    
  //main loop 

while (1) {

    if (option == 'd' || option == 'D' || leaveLog == 'q' || leaveLog == 'Q') { //log out or return from an option then this brings you back here 
        goto theloop;
    }

theloop:
    printf("Welcome to Simple Bank System!\nWhat would you like to do?\nA.Login\nB.Signup\nC.Quit\n");
    scanf(" %c", &choice);

    if (choice == 'a' || choice == 'A') {                           
        Login(numaccount, deposit, withdraw, option, leaveLog); //Login
    } else if (choice == 'b' || choice == 'B') {
        CreateNewAccount(newname, randNum, accounts, users); //signup
    } else if (choice == 'c' || choice == 'C') {            
        return 1;                                          //quit function
    } else {
        clearScreen();
        printf("Please choose one of the options provided\n\n"); //if correct option isnt picked
    }
}
return 0;
}


//$$\   $$\  $$$$$$\ $$$$$$$$\ $$$$$$$$\  $$$$$$\  
//$$$\  $$ |$$  __$$\\__$$  __|$$  _____|$$  __$$\ 
//$$$$\ $$ |$$ /  $$ |  $$ |   $$ |      $$ /  \__|
//$$ $$\$$ |$$ |  $$ |  $$ |   $$$$$\    \$$$$$$\     
//$$ \$$$$ |$$ |  $$ |  $$ |   $$  __|    \____$$\ 
//$$ |\$$$ |$$ |  $$ |  $$ |   $$ |      $$\   $$ |
//$$ | \$$ | $$$$$$  |  $$ |   $$$$$$$$\ \$$$$$$  |
//\__|  \__| \______/   \__|   \________| \______/ 


//Accounts.txt must NOT have any new lines under the bottom most account(3 accounts means 3 lines of text)
//                                                                            _________________  
//                                                                           |                |
//Balance.txt MUST have a blank line at the top of the file like ------>     |555555 = $100.00|
//                                                                           |444444 = $150.00| 
//overall just dont edit the txt files manually                              |----------------|    