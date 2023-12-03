#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<ctype.h>
#define AC_RED "\x1b[31m"
#define AC_NORMAL "\x1b[m"
#define AC_GREEN "\x1b[32m"
#define AC_YELLOW "\e[1;33m"
#define BHYEL "\e[1;93m"

int numUsers = 0, countUsers = 0, cUser = 0, admin = 0;
char CurrentUser[100];

char *getCurrentTime()
{
    time_t currentTime;
    time(&currentTime);
    struct tm *timeInfo = localtime(&currentTime);

    static char timeStr[20];

    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M", timeInfo);

    return timeStr;
}

struct node
{
    char first_name[50];
    char last_name[50];
    char email[50];
    char username[50];
    char phone[50];
    char address[50];
    char age[20];
    char gender[20];
    char pass1[50];
    // Bank struct
    int balance;
    int d_amount;
    struct history *hiss;
    struct node *next;
} *head = NULL;

struct history
{
    int data[5];
    char times[5][20];
    int type[5];
    char id[50];
    struct history *next;
} *start = NULL;

//-----------------------------------------File---------------------------------------------------------------------------------------

void writeFile()
{
    FILE *file = fopen("userdata.txt", "w");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    struct node *current = head;

    while (current != NULL)
    {
        fprintf(file, "%s|%s|%s|%s|%s|%s|%s|%s|%s|%d|\n", current->first_name, current->last_name, current->email, current->username, current->phone, current->address, current->age, current->gender, current->pass1, current->balance);
        current = current->next;
    }

    fclose(file);
}

void readFile()
{
    FILE *file = fopen("userdata.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    struct node *current = NULL;
    struct history *track = NULL;
    char line[1000];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        char first_name[50];
        char last_name[50];
        char email[50];
        char username[50];
        char phone[50];
        char address[50];
        char age[20];
        char gender[20];
        char pass1[50];
        int balance;

        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|", first_name, last_name, email, username, phone, address, age, gender, pass1, &balance);
        struct node *newNode = malloc(sizeof(struct node));
        track = start;

        strcpy(newNode->first_name, first_name);
        strcpy(newNode->last_name, last_name);
        strcpy(newNode->email, email);
        strcpy(newNode->username, username);
        strcpy(newNode->phone, phone);
        strcpy(newNode->address, address);
        strcpy(newNode->age, age);
        strcpy(newNode->pass1, pass1);
        strcpy(newNode->gender, gender);
        newNode->balance = balance;
        newNode->hiss = track;
        track = track->next;

        numUsers++;
        newNode->next = NULL;

        if (head == NULL)
        {
            head = newNode;
            current = newNode;
        }
        else
        {
            current->next = newNode;
            current = newNode;
        }
    }
    printf("OKAY\n");
    fclose(file);
}

void writeHistoryFile()
{
    int a;
    FILE *file = fopen("userHistory.txt", "w");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    struct history *track = start;

    while (track != NULL)
    {
        a = fprintf(file, "%d|%s|%d|%s|%d|%s|%d|%s|%d|%s|%d|%s|%d|%s|%d|%s|%d|%s|%d|%s|\n",
                    track->data[0], track->times[0], track->type[0], track->id,
                    track->data[1], track->times[1], track->type[1], track->id,
                    track->data[2], track->times[2], track->type[2], track->id,
                    track->data[3], track->times[3], track->type[3], track->id,
                    track->data[4], track->times[4], track->type[4], track->id);

        track = track->next;
    }

    fclose(file);
}

void readHistoryFile()
{
    FILE *file = fopen("userHistory.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    struct history *track = start;
    char line[1000];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        struct history *current, *track = malloc(sizeof(struct history));
        track->next = NULL;
        int result = sscanf(line, "%d|%[^|]|%d|%[^|]|%d|%[^|]|%d|%[^|]|%d|%[^|]|%d|%[^|]|%d|%[^|]|%d|%[^|]|%d|%[^|]|%d|%[^|]|\n",
                            &track->data[0], track->times[0], &track->type[0], track->id,
                            &track->data[1], track->times[1], &track->type[1], track->id,
                            &track->data[2], track->times[2], &track->type[2], track->id,
                            &track->data[3], track->times[3], &track->type[3], track->id,
                            &track->data[4], track->times[4], &track->type[4], track->id);

        if (result != 20)
        {
            printf("Error reading line from history file.\n");
            free(track);
            continue;
        }

        if (start == NULL)
        {
            start = track;
            current = track;
        }
        else
        {
            current->next = track;
            current = track;
        }
    }

    fclose(file);
}

void divider()
{
    printf("\t\t\t");
    for (int i = 0; i < 57; i++)
    {
        printf("-");
    }
    printf("\n");
}

//----------------------------------------password-----------------------------------------------------------------------------------

void encryptPassword(const char *password, char *encryptedPassword)
{
    int length = strlen(password);

    for (int i = 0; i < length; i++)
    {
        encryptedPassword[i] = password[i] + 29;
    }
    encryptedPassword[length] = '\0';
}

void decryptPassword(const char *encryptedPassword, char *decryptedPassword)
{

    for (int i = 0; i < strlen(encryptedPassword); i++)
    {
        decryptedPassword[i] = encryptedPassword[i] - 29;
    }
    decryptedPassword[strlen(encryptedPassword)] = '\0';
}

void takePassword(char pass[20])
{
    int i = 0;
    char ch;

    while (1)
    {
        ch = getch();

        if (ch == 13)
        {
            pass[i] = '\0';
            break;
        }
        else if (ch == 8)
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else if (ch == 9 || ch == 32)
        {
            continue;
        }
        else
        {
            pass[i++] = ch;
            printf("*");
        }
    }
}

void addRandomNumbersToString(char *str)
{
    srand(time(0));
    int len = strlen(str);
    for (int i = len; i < len + 4; i++)
    {
        str[i] = '0' + rand() % 10;
    }
    str[len + 4] = '\0';
}

char generateAccountName(char email[50], char username[50])
{
    int i;
    for (i = 0; i < strlen(email); i++)
    {
        if (email[i] == '@')
            break;
        else
        {
            username[i] = email[i];
        }
    }
    username[i] = '\0';
    addRandomNumbersToString(username);
}

void change_password()
{
    system("cls");
    char pwd[20];
    char pass1[20], pass2[20], password[20], en_password[20];
    int num1, num2;
    struct node *temp;
    srand(time(0));

    printf("\t\t\t-----------------> Change your password <----------------\n");
    divider();
    printf("\n\t\t\tAre you a human? ( y/n ) >> ");
    char ch = getch();

    temp = head;

code:
    num1 = rand();
    if (ch == 'Y' || ch == 'y')
    {
        printf("\n\n\t\t\tThe verify number is %d\n", num1);

        printf("\n\n\t\t\tEnter the above number :  ");
        scanf("%d", &num2);
        if (num1 == num2)
        {
            {
                while (strcmp(temp->username, CurrentUser))
                {
                    temp = temp->next;
                    if (temp == NULL)
                    {
                        printf("%s\t\t\t\tUser not found%s\n", AC_RED, AC_NORMAL);
                        return;
                    }
                }
                printf("\n\t\t\tEnter password: ");
                takePassword(pwd);
                decryptPassword(temp->pass1, password);
                if (strcmp(password, pwd) == 0)
                {
                pwd2:
                    printf("\n\n\t\t\tEnter new password: ");
                    scanf("%s",pass1);
                    printf("\n\t\t\tConfirm new password: ");
                    scanf("%s", pass2);
                    if (strcmp(pass1, pass2) == 0 && strlen(pass1) >= 4)
                    {
                        encryptPassword(pass2, en_password);
                        strcpy(temp->pass1, en_password);
                        printf("\n\n\t\t\t%sPassword changed successfully.%s\n", AC_GREEN, AC_NORMAL);
                        printf("\n    \t\t\tPress Any Key to Return!!\n");

                        writeFile();
                        getch();
                        dashboard();
                    }
                    else
                    {
                        printf("\n\t\t\t%sPassword should contain atleast 4 characters.%s", AC_RED, AC_NORMAL);

                        goto pwd2;
                    }
                }
                else
                {
                    printf("\n\t\t\t%sPLEASE ENTER CORRECT PASSWORD%s\n", AC_RED, AC_NORMAL);
                }
            }
        }
        else
        {
            printf("\n\n\t\t\t%sPlease enter correct code.%s\n", AC_RED, AC_NORMAL);

            goto code;
        }
    }
    else
    {
        printf("\n");
    }
}

//--------------------------------------Authentication-------------------------------------------------------------------------------
int isValidNumber(const char *number) {

    int length = strlen(number);
    if (length != 11)
        return 0;

    if (number[0] != '0' || number[1] != '1')
        return 0;

    if (number[2] < '3' || number[2] > '9')
        return 0;

    for (int i = 3; i < length; i++) {
        if (!isdigit(number[i]))
            return 0;
    }

    return 1;
}

int isValidGmail(const char *email) {
    int atIndex = -1;
    int dotIndex = -1;
    int length = strlen(email);

    for (int i = 0; i < length; i++) {
        if (email[i] == '@') {
            atIndex = i;
        } else if (email[i] == '.') {
            dotIndex = i;
        } else if(email[i] == ' ') {
            return 0;
        }
    }

    if (atIndex > 0 && dotIndex > atIndex+1 && dotIndex < length - 1) {
        const char *domain = email + dotIndex + 1;
        if (strcmp(domain, "com") == 0) {
            return 1;
        }
    }else{
        return 0;
    }
}

void signup()
{

    system("cls");
    char pass2[50];
    char password[20];
    int len,bal;
    struct node *new_node, *current;
    struct history *history_node, *t_history;
    new_node = malloc(sizeof(struct node));
    history_node = malloc(sizeof(struct history));
    for (int i = 0; i < 5; i++)
    {
        history_node->data[i] = 0;
        strcpy(history_node->times[i], "0");
        strcpy(history_node->id, "0");
        history_node->type[i] = 0;
    }

    new_node->next = NULL;
    history_node->next = NULL;

    if (new_node == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("\t\t\t\t---------- Create New Account ----------\n\n");
    divider();
    printf("\nEnter your first name  : ");
    scanf(" %[^\n]", new_node->first_name);
    printf("Enter your last name   : ");
    scanf(" %[^\n]", new_node->last_name);

    reE:
    printf("Enter your email       : ");
    scanf(" %[^\n]", new_node->email);

    if(isValidGmail(new_node->email)==0){
        printf("%sInviled Email%s\n",AC_RED, AC_NORMAL);
        getch();
        goto reE;
    }

    re_num:
    printf("Enter your phone number: ");
    scanf(" %s", new_node->phone);
    len = strlen(new_node->phone);
    if(len < 11){
        printf("%sPhone Number is short by %d digit.%s\n",AC_RED, 11-len, AC_NORMAL);
        getch();
        goto re_num;
    }else if(len > 11 || isValidNumber(new_node->phone)==0){
        printf("%sInviled Number%s\n",AC_RED, AC_NORMAL);
        getch();
        goto re_num;
    }

    printf("Enter your address     : ");
    scanf(" %[^\n]", new_node->address);
    printf("Enter your gender      : ");
    scanf(" %s", new_node->gender);
    printf("Enter your age         : ");
    scanf("%s", new_node->age);

    bal:
    printf("Enter open balance     : ");
    scanf("%d", &new_node->balance);
    bal = new_node->balance;
    if(bal < 500){
        printf("%sBalance is lower then 500 taka.%s\n",AC_RED, AC_NORMAL);
        getch();
        goto bal;
    }

    new_node->hiss = history_node;

pwd:
    printf("\nEnter Password         : ");
    takePassword(password);

    if (strlen(password) >= 4)
    {
    conf_pwd:
        printf("\nConfirm Password       : ");
        takePassword(pass2);

        if (strcmp(password, pass2))
        {
            printf("\n\n%s---------- INCORRECT PASSWORD ----------%s\n", AC_RED, AC_NORMAL);

            goto conf_pwd;
        }
        else
        {
            system("cls");
            encryptPassword(pass2, password);
            strcpy(new_node->pass1, password);
            generateAccountName(new_node->email, new_node->username);
            printf("\n\n\t\t\t%s>>>> ACCOUNT CREATED SUCCESSFULLY <<<<\n\n\t\t\t>>>> Your Account name is: %s <<<<%s\n", AC_GREEN, new_node->username, AC_NORMAL);
            printf("\n\n\t\t\t%s**** Registration successful! ****%s\n", AC_GREEN, AC_NORMAL);
            printf("\n\t\t\t\tPress Any Key to Return!!\n");

            if (head == NULL)
            {
                head = new_node;
                start = history_node;
                current = new_node;
                numUsers++;
            }
            else
            {
                current = head;
                t_history = start;
                while (current->next != NULL)
                {
                    t_history = t_history->next;
                    current = current->next;
                }
                current->next = new_node;
                t_history->next = history_node;
                numUsers++;
            }
            writeFile();
            writeHistoryFile();
            getch();
        }
    }
    else
    {
        printf("%s\nPassword should contain atleast 4 characters.!!!%s", AC_RED, AC_NORMAL);

        goto pwd;
    }
}

void userDetails()
{
    system("cls");
    divider();
    struct node *temp;
    temp = head;

    for (int i = 1; i < countUsers; i++)
    {
        temp = temp->next;
    }
    if(admin != 1){
        printf("\t\t\t\t%s ************ WELCOME %s %s ************%s\n", BHYEL, temp->first_name, temp->last_name, AC_NORMAL);
        divider();
    }
    printf("\t\t\t\t\t%s>>>> Your Details <<<<%s\n", BHYEL, AC_NORMAL);
    divider();
    printf("\n\t\t\t\tAccount Name : %s\n", temp->username);
    printf("\t\t\t\tFull name    : %s %s\n", temp->first_name, temp->last_name);
    printf("\t\t\t\temail        : %s\n", temp->email);
    printf("\t\t\t\tphone number : +88%s\n", temp->phone);
    printf("\t\t\t\tGender       : %s\n", temp->gender);
    printf("\t\t\t\tAddress      : %s\n", temp->address);
    printf("\t\t\t\tAge          : %s years\n\n", temp->age);
    printf("\t\t\t\t%sBalance      : %d taka%s\n\n", BHYEL, temp->balance, AC_NORMAL);

    divider();
    return;
};

void login()
{
    system("cls");
    struct node *temp, *current = head;
    char password[20];
    temp = NULL;
    char user1[30], log_pass[20];
    countUsers = 0;

    printf("\n\t\t\t\t************ LOG IN FIRST ************\n\n");
    divider();

    printf("\n\t\t\tEnter Account Name : ");
    scanf("%s", user1);

    // Admin Check
    if (strcmp(user1, "admin") == 0)
    {
        printf("\t\t\tEnter password\t   : ");
        takePassword(log_pass);
        if (strcmp(log_pass, "admin") == 0)
        {
            admin = 1;
            dashboard();
            return;
        }
    }

    while (strcmp(user1, current->username) != 0)
    {
        current = current->next;
        if (current == NULL)
        {
            printf("%s\t\t\t\tNo User found%s\n", AC_RED, AC_NORMAL);
            printf("\t\t\t\tPress enter again.\n");
            getch();

            return;
        }
    }

    temp = head;
    while (temp != NULL)
    {
        countUsers++;
        if (strcmp(temp->username, user1) == 0)
        {
            break;
        }
        temp = temp->next;
    }
pwd:
    printf("\t\t\tEnter password\t   : ");
    takePassword(log_pass);
    decryptPassword(current->pass1, password);

    if (!strcmp(password, log_pass))
    {
        strcpy(CurrentUser, temp->username);
        dashboard();
    }
    else
    {
        printf("\n\t\t\t%sPLEASE ENTER CORRECT PASSWORD%s\n", AC_RED, AC_NORMAL);

        goto pwd;
    }
}

void search()
{
    system("cls");
    char choice;
    int num = 0, notFound = 1;
    char username[20], password[20], phone[20];

    printf("\n\t\t************ Search your account with PASSWORD OR USERNAME ************\n\n");
    divider();
    printf("\t\t\t| 1. Search by Username.\t\t\t\t|\n");
    printf("\t\t\t| 2. Search by Phone no.\t\t\t\t|\n");
    printf("\t\t\t| 3. MAIN MENU\t\t\t\t\t\t|\n");
    divider();

    struct node *temp;

ch:
    countUsers = 0;
    printf("\nEnter your choice : ");
    scanf(" %c", &choice);
    switch (choice)
    {
    case '1':
        printf("\nEnter your Username : ");
        scanf("%s", username);

        temp = head;
        while (temp != NULL)
        {
            countUsers++;
            if (strcmp(temp->username, username) == 0)
            {
                printf("%sACCOUNT FOUND!%s\n", AC_GREEN, AC_NORMAL);
                getch();
                userDetails();
                printf("\n\t\t\t\tPlease press enter to return the start page.");
                getch();
                notFound = 0;
            }
            temp = temp->next;
        }

        if (notFound == 1)
        {
            printf("\n%sUSERNAME is NOT FOUND !!%s\n\n", AC_RED, AC_NORMAL);
            goto ch;
        }
        break;

    case '2':
        printf("\nEnter your Phone : ");
        scanf("%s", phone);

        temp = head;

        while (temp != NULL)
        {
            countUsers++;
            if (strcmp(temp->phone, phone) == 0)
            {
                num++;
                if(num>1){
                    printf("%s\t\t\t\tAnother ACCOUNT FOUND With Same Number!%s\n", AC_GREEN, AC_NORMAL);
                    getch();
                }
                userDetails();
                notFound = 0;
            }
            temp = temp->next;
        }
        
        printf("\n\t\t\t\tPlease press enter to return the start page.");
        getch();

        if (notFound == 1)
        {
            printf("\n%sPHONE NUMBER is NOT FOUND !!%s\n\n", AC_RED, AC_NORMAL);

            goto ch;
        }
        break;

    case '3':
        return;

    default:
        system("cls");
        printf("\n\n\t\t\t%sINVALID CHOICE and Press ENTER to back.%s\n", AC_RED, AC_NORMAL);
        getch();

        search();
        break;
    }
}

void delete_account()
{
    system("cls");
    char pwd[20];
    char user1[20];
    char password[20];

    printf("\t\t\t-----------------> Delete your account <-----------------\n");
    divider();

    printf("\n\t\t\tEnter username: ");
    scanf("%s", user1);

    struct node *temp;
    struct history *temp2;

    temp = head;
    temp2 = start;

    if (!strcmp(temp->username, user1))
    {
        printf("\n\t\t\tEnter password: ");
        scanf("%s", pwd);
        decryptPassword(temp->pass1, password);
        if (strcmp(password, pwd) == 0)
        {
            head = temp->next;
            start = temp2->next;
            numUsers--;
        }
        else
        {
            printf("\n\t\t\t%sPLEASE ENTER CORRECT PASSWORD%s\n", AC_RED, AC_NORMAL);
        }
    }
    else
    {
        while (strcmp(temp->next->username, user1) != 0)
        {
            temp = temp->next;
            temp2 = temp2->next;
            if (temp->next == NULL)
            {
                printf("%s\t\t\t\tUser not found%s\n", AC_RED, AC_NORMAL);
                printf("\n\t\t\t  Press Any Key to Return!!\n");
                getch();
                return;
            }
        }
    pw:
        printf("\n\t\t\tEnter password: ");
        takePassword(pwd);
        decryptPassword(temp->next->pass1, password);
        if (strcmp(password, pwd) == 0)
        {
            temp->next = temp->next->next;
            temp2->next = temp2->next->next;
            numUsers--;
            printf("%s\n\t\t\t\t\tAccount Delete Successfully!%s\n", AC_GREEN, AC_NORMAL);
            printf("\n   \t\t\t\t\tPress Any Key to Return!!\n");
            getch();
            writeFile();
            writeHistoryFile();
            return;
        }

        else
        {
            printf("\n\t\t\t%sPLEASE ENTER CORRECT PASSWORD%s\n", AC_RED, AC_NORMAL);
            goto pw;
        }
    }
    printf("%s\n\t\t\t\t\tAccount Delete Successfully!%s\n", AC_GREEN, AC_NORMAL);
    printf("\n   \t\t\t\t\tPress Any Key to Return!!\n");
    getch();
    writeFile();
    writeHistoryFile();
}

// -----------------------------------------------------------------------------------------------------------------------------

int getFirstDayOfTheYear(int year)
{
    int day = (year * 365 + ((year - 1) / 4) - ((year - 1) / 100) + ((year - 1) / 400)) % 7;
    return day;
}

int calender()
{
    system("cls");
    char *month[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int i, j, totalDays, weekDay = 0, spaceCounter = 0, year;

    printf("Enter Year: ");
    scanf("%d", &year);

    printf("\n\n\t\t\t\t\t>>>>>>>> Welcome to %d <<<<<<<<", year);

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
        daysInMonth[1] = 29;
        printf("\n\n\t\t\t\t%s>>>>>>>> Hurrah!! It's a Leap Year <<<<<<<<%s", AC_GREEN, AC_NORMAL);
    }

    weekDay = getFirstDayOfTheYear(year);

    for (i = 0; i < 12; i++)
    {
        printf("\n\n\n------------------------- %s -------------------------\n", month[i]);
        printf("\n      Sun     Mon     Tue     Wed     Thu     Fri     Sat\n\n");

        for (spaceCounter = 1; spaceCounter <= weekDay; spaceCounter++)
        {
            printf("        ");
        }

        totalDays = daysInMonth[i];
        for (j = 1; j <= totalDays; j++)
        {
            printf("%8d", j);
            weekDay++;
            if (weekDay > 6)
            {
                weekDay = 0;
                printf("\n");
            }
        }
    }

    getch();
    dashboard();
}

// -----------------------------------------------------------------------------------------------------------------------------

void transection_history()
{
    system("cls");
    printf("%s\n\t\t\t\t\t\tHistory\n%s", AC_YELLOW, AC_NORMAL);
    divider();
    printf("\n");
    struct history *current = start;

    if (current == NULL)
    {
        printf("The list is empty.\n");
        return;
    }

    for (int i = 0; i < countUsers - 1; i++)
    {
        current = current->next;
    }

    for (int i = 0; i < 5; i++)
    {

        if (current->type[i] == 1)
        {
            printf("\t\t\tTransaction[%d]: ", i + 1);
            printf("Deposit of %s%d Tk%s at %s\n", AC_GREEN, current->data[i], AC_NORMAL, current->times[i]);
        }
        else if (current->type[i] == 2)
        {
            printf("\t\t\tTransaction[%d]: ", i + 1);
            printf("Withdrawal of %s%d Tk%s at %s\n", AC_RED, current->data[i], AC_NORMAL, current->times[i]);
        }
        else if (current->type[i] == 3)
        {
            printf("\t\t\tTransaction[%d]: ", i + 1);
            printf("Transfer of %s%d Tk%s at %s to %s%s%s\n", AC_RED, current->data[i], AC_NORMAL, current->times[i], AC_YELLOW, current->id, AC_NORMAL);
        }
        else if (current->type[i] == 4)
        {
            printf("\t\t\tTransaction[%d]: ", i + 1);
            printf("Recieved %s%d Tk%s at %s From %s%s%s\n", AC_GREEN, current->data[i], AC_NORMAL, current->times[i], AC_YELLOW, current->id, AC_NORMAL);
        }
        else
        {
            printf("\n");
        }
    }
    printf("\n  \t\t\t\t\tPress Any Key to Return!!\n");
    getch();
    dashboard();
    return;
}

void deposit_money()
{
    struct node *current = head;
    struct history *track = start;
    int cash;
    reAm:
    system("cls");
    printf("%s\n\t\t\t\t\t\tDeposit Money\n%s", AC_YELLOW, AC_NORMAL);
    divider();
    printf("\n");


    printf("\t\t\tEnter Amount: ");
    scanf("%d", &cash);

    if (cash < 1)
    {
        printf("\n\t\t\t%sToo low%s", AC_RED, AC_NORMAL);
        getch();
        goto reAm;
    }

    if (track == NULL)
    {
        printf("Error: Current or track pointers are NULL.\n");
        getch();
        return;
    }

    for (int i = 0; i < countUsers - 1; i++)
    {
        current = current->next;
        track = track->next;
    }
    current->balance += cash;
    for (int i = 4; i > 0; i--)
    {
        track->data[i] = track->data[i - 1];
        strcpy(track->times[i], track->times[i - 1]);
        track->type[i] = track->type[i - 1];
    }
    track->data[0] = cash;
    track->type[0] = 1;
    strcpy(track->times[0], getCurrentTime());
    writeFile();
    writeHistoryFile();
    printf("%s\n\t\t\t\t\tMoney Deposit Successful!!%s\n", AC_GREEN, AC_NORMAL);
    printf("\n  \t\t\t\t\tPress Any Key to Return!!\n");
    getch();
    dashboard();
}

void withdraw_money()
{
    struct node *current = head;
    struct history *track = start;
    int cash;
    char pass[20], password[20];
    reAm:
    system("cls");
    printf("%s\n\t\t\t\t\t\tWithdraw Money\n%s", AC_YELLOW, AC_NORMAL);
    divider();
    printf("\n");    
    
    printf("\t\t\tEnter Amount: ");
    scanf("%d", &cash);
    if (cash < 100)
    {
        printf("\n\t\t\t%sToo low%s\n", AC_RED, AC_NORMAL);
        getch();
        goto reAm;
    }

    for (int i = 0; i < countUsers - 1; i++)
    {
        current = current->next;
        track = track->next;
    }
pas:
    printf("\t\t\tEnter Password: ");
    takePassword(pass);
    decryptPassword(current->pass1, password);
    if (!strcmp(pass, password))
    {
        if (current->balance < cash)
        {
            printf("%s\n\t\t\t\t\tInsufficient balance!!%s\n", AC_RED, AC_NORMAL);
            getch();
            dashboard();
            return;
        }
        current->balance -= cash;

        for (int i = 4; i > 0; i--)
        {
            track->data[i] = track->data[i - 1];
            strcpy(track->times[i], track->times[i - 1]);
            track->type[i] = track->type[i - 1];
        }

        track->data[0] = cash;
        track->type[0] = 2;
        strcpy(track->times[0], getCurrentTime());
    }
    else
    {
        printf("%s\n\t\t\t\t\tWrong Password!!%s\n", AC_RED, AC_NORMAL);
        goto pas;
    }
    printf("%s\n\t\t\t\t\tMoney Withdrawn Successfully!!%s\n", AC_GREEN, AC_NORMAL);
    printf("\n    \t\t\t\t\tPress Any Key to Return!!\n");
    writeFile();
    writeHistoryFile();
    getch();
    dashboard();
}

void transfer_money()
{
    struct node *user1 = head, *current = head;
    struct history *track = start, *track2 = start;
    int cash;
    char pass[20], user[50], password[20];
    system("cls");
    printf("%s\n\t\t\t\t\t\tTransfer Money\n%s", AC_YELLOW, AC_NORMAL);
    divider();
    printf("\n");

    printf("\t\t\tReciver User Id: ");
    scanf("%s", user);
    while (strcmp(user1->username, user))
    {
        user1 = user1->next;
        track = track->next;
        if (user1 == NULL)
        {
            printf("\n\t\t\t\t\t%sUser NOT FOUND!!%s\n", AC_RED, AC_NORMAL);
            getch();
            dashboard();
            return;
        }
    }
    if (strcmp(user, CurrentUser) == 0)
    {
        printf("\n\t\t\t\t%sAccess Declined%s\n", AC_RED, AC_NORMAL);
        getch();
        dashboard();
        return;
    }
    reAm:
    printf("\t\t\tEnter Amount: ");
    scanf("%d", &cash);
    if (cash < 1)
    {
        printf("\n\t\t\t%sToo low%s\n", AC_RED, AC_NORMAL);
        goto reAm;
    }

    for (int i = 0; i < countUsers - 1; i++)
    {
        current = current->next;
        track2 = track2->next;
    }

pas:
    printf("\t\t\tEnter Password: ");
    takePassword(pass);
    decryptPassword(current->pass1, password);

    if (!strcmp(pass, password))
    {
        if (current->balance < cash)
        {
            printf("%s\n\t\t\t\t\tInsufficient balance!!%s\n", AC_RED, AC_NORMAL);
            getch();
            dashboard();
            return;
        }
        current->balance -= cash;
        user1->balance += cash;
        strcpy(track->id, current->username);
        strcpy(track2->id, user1->username);

        for (int i = 4; i > 0; i--)
        {
            track->data[i] = track->data[i - 1];
            strcpy(track->times[i], track->times[i - 1]);
            track->type[i] = track->type[i - 1];

            track2->data[i] = track2->data[i - 1];
            strcpy(track2->times[i], track2->times[i - 1]);
            track2->type[i] = track2->type[i - 1];
        }

        track->data[0] = cash;
        track->type[0] = 4;
        strcpy(track->times[0], getCurrentTime());

        track2->data[0] = cash;
        track2->type[0] = 3;
        strcpy(track2->times[0], getCurrentTime());
    }
    else
    {
        printf("%s\n\t\t\t\t\tWrong Password!!%s\n\n", AC_RED, AC_NORMAL);
        goto pas;
    }
    printf("%s\n\t\t\t\t\tMoney Transfered Successfully!!%s\n", AC_GREEN, AC_NORMAL);
    printf("\n    \t\t\t\t\tPress Any Key to Return!!\n");
    writeFile();
    writeHistoryFile();
    getch();
    dashboard();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------

void about_us(){
    system("cls");
    printf("\t\t\t\t\t\tTeam Members\n");
    divider();
    printf("\n\n\t\t\tName \t\t\t ID \t\t\t Email\n\n");
    printf("\t\t\tUtsho Roy \t\t 0242310005101244 \t roy23105101244@diu.edu.bd\n");
    printf("\t\t\tAshfikur Rahman Mahi \t 0242310005101618 \t mahi2305101618@diu.edu.bd\n");
    printf("\t\t\tAshfakur Rahman \t 0242310005101389 \t ramin2305101389@diu.edu.bd\n");
    printf("\t\t\tImteaz Ahmead \t\t 0242310005101280 \t ahamed23105101280@diu.edu.bd\n");
    printf("\n\t\t\t\t\t %sDaffodil International University%s\n",BHYEL, AC_NORMAL);
    getch();
}

void dashboard()
{
    if (admin == 1)
    {
        re:
        system("cls");
        char c;
        printf("\t\t\t\t\t  %s $$$ Bank Options $$$%s\n", AC_YELLOW, AC_NORMAL);
        divider();
        printf("\t\t\t\t\t     Welcome Admin\n");
        printf("\t\t\t\t  ******** Total Accounts: %d ********\n", numUsers);
        printf("\n\t\t\t\t1. Search");
        printf("\n\t\t\t\t2. Calender");
        printf("\n\t\t\t\t3. Logout\n\n");
        printf("\n\t\t\t\tEnter your choice: ");
        scanf(" %c", &c);
        switch (c)
        {
        case '1':
            if(numUsers==0){
                printf("\t\t\t\t%sBank Has no Users%s",AC_RED, AC_NORMAL);
                getch();
                goto re;
                break;
            }else{
                search();
            }
            goto re;
            break;
        case '2':
            calender();
            break;
        case '3':
            admin = 0;
            return;

        default:
            system("cls");
            printf("\n\t\t\t\t\t\t%sInvalid choice!!%s\n\n\n\t\t\t\t\t\tPlease press enter.", AC_RED, AC_NORMAL);
            getch();
            goto re;
            break;
        };
    }
    else
    {
        char c;
        again:
        userDetails();
        printf("\t\t\t\t\t%s $$$ Bank Options $$$%s\n", AC_YELLOW, AC_NORMAL);
        divider();
        printf("\n\t\t\t\t1. Calender");
        printf("\n\t\t\t\t2. Deposit Money");
        printf("\n\t\t\t\t3. Withdraw Money");
        printf("\n\t\t\t\t4. Transfer Money");
        printf("\n\t\t\t\t5. History");
        printf("\n\t\t\t\t6. Change Password");
        printf("\n\t\t\t\t7. Delete Account");
        printf("\n\t\t\t\t8. Log out\n\n");
        divider();
        printf("\n\t\t\t\tEnter your choice: ");
        scanf(" %c", &c);
        switch (c)
        {
        case '1':
            calender();
            break;
        case '2':
            deposit_money();
            break;
        case '3':
            withdraw_money();
            break;
        case '4':
            transfer_money();
            break;
        case '5':
            transection_history();
            break;
        case '6':
            change_password();
            break;
        case '7':
            delete_account();
            countUsers = 0;
            return;
        case '8':
            countUsers = 0;
            return;

        default:
            system("cls");
            printf("\n\t\t\t\t\t\t%sInvalid choice!!%s\n\n\n\t\t\t\t\t\tPlease press enter.", AC_RED, AC_NORMAL);
            getch();
            goto again;

            break;
        }
    }
}

char menu()
{
    system("cls");
    char c;
    printf("\t\t\t\t   ******** Welcome to Bank ********\n");
    printf("\n\t\t\t---------------------------------------------------------\n\t\t\t|\t\t\t\t\t\t\t|");
    printf("\n\t\t\t|\t1. SIGNUP\t\t\t\t\t|\n");
    printf("\t\t\t|\t2. LOGIN\t\t\t\t\t|\n");
    printf("\t\t\t|\t3. ABOUT US\t\t\t\t\t|\n");
    printf("\t\t\t|\t4. EXIT\t\t\t\t\t\t|\n\t\t\t|\t\t\t\t\t\t\t|");
    printf("\n\t\t\t---------------------------------------------------------\t\t\n");
    printf("\n>> Enter your choice: ");
    scanf(" %c", &c);

    return c;
}

int main()
{
    if (head == NULL)
    {
        readHistoryFile();
        readFile();
    }
    char choice;
    while (1)
    {

        choice = menu();

        switch (choice)
        {
        case '1':
            signup();
            break;
        case '2':
            login();
            break;
        case '3':
            about_us();
            break;
        case '4':
            exit(0);
            break;

        default:
            printf("\n%sInvalid option. Please select right option.%s\n", AC_RED, AC_NORMAL);
            getch();
            system("cls");
            break;
        }
    }
    return 0;
}
