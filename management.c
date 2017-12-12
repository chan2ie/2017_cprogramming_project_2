#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <malloc.h>
#include <time.h>

#define MENU_ASSIGN '1'
#define MENU_CGPA '2'
#define MENU_CHANGE '3'
#define MENU_LOGOUT '4'
#define MENU_LOGIN '1'
#define MENU_NEW '2'
#define MENU_DEL '3'
#define MENU_TEMP '4'
#define MENU_QUIT '5'
#define CGPA_ADD '1'
#define CGPA_VIEW '2'
#define CGPA_QUIT '3'
#define LOGIN_FAILED 7
#define LOGIN_SCCUESS 1
#define EXIT 5
#define LOGOUT 4
#define INVAILD_CHOICE 6
#define NEW_ACCOUNT_FAIL 21
#define NEW_ACCOUNT_SUCCESS 2
#define DELETE_ACCOUNT_FAIL_INVALID 31
#define DELETE_ACCOUNT_FAIL_PASSWORD 32
#define DELETE_ACCOUNT_SUCCESS 3
#define MAIN_MENU 23
#define CHANGE_PASSWORD_SUCCESS 131
#define CHANGE_PASSWORD_FAIL 132

char Curr_Num[9];
char Curr_Pass[16];
char New_Num[9];
char New_Pass[16];
int Curr_year_index;
int Curr_num_index;

typedef struct CHILD1{
  char name[100];
  char describe[100];
  char professor[100];
  int date[2];
  struct CHILD1 *link;
}ASSIGN;

typedef struct CHILD2{
  int semester;
  float score;
  struct CHILD2 *link;
}CGPA;

typedef struct SUB_SUB_HEAD{
  char number[5];
  char password[16];
  ASSIGN* Child_A;
  int Assign_Size;
  int CGPA_Size;
  CGPA* Child_C;
  struct SUB_SUB_HEAD *link;
}STUDENT;

typedef struct SUB_HEAD{
  char year[5];
  int Num_Size;
  STUDENT* ST_NUM;
}YEAR;

typedef struct HEAD{
  int Year_Size;
  YEAR* ST_YEAR;
}TREE_HEAD;

int Login_Year;
int Login_Num;
int Year_Count=8;
int Student_Count;
int Assign_Count;
int CGPA_Count;

TREE_HEAD* TOP;
void Load_Data ();
void Save_Data ();
char menu ();
char cgpa_menu ();
void Add_GPA ();
void Cor_GPA (STUDENT *login_st, char semester,float gpa);
void Create_Struct ();
void Print_Assign( int);
void Print_CGPA ();
void Print_CGPA_Graph ();
int New_Account ();
int Delete_Account ();
int Change_Password ();
void rank ();
void Sort_Assign ();
void Add_Assign ();
void Delete_Assign ();
int login ();
void Temp_Password ();
char Account_Manage ();
void read_data (FILE * fpoint);
void create_student ();
void create_assign (); 
void create_cgpa (CGPA *new, int semester, float score);
int search_year ();
STUDENT*  search_num ();
int run_account_manage ();
int run_menu ();
int run_cgpa_menu ();
void free_student ();
void free_assign ();
void free_cgpa ();

int main(){
    int exit=0;
    int login_flag=0;

    STUDENT *login_st;

    initscr();

    Create_Struct();

    while(!exit){ 
        switch(run_account_manage(login_st)){
            case LOGIN_FAILED :
                clear();
                printw("Login Failed!\n\n");
                continue;
            case NEW_ACCOUNT_SUCCESS :
                clear();
                printw("New account Succesfully made!\n\n");
                continue;
            case NEW_ACCOUNT_FAIL :
                clear();
                printw("Failed to create new accont. (Existing account)\n\n");
                continue;
            case DELETE_ACCOUNT_SUCCESS :
                clear();
                printw("Succesfully deleted account!\n\n");
                continue;
            case DELETE_ACCOUNT_FAIL_INVALID :
                clear();
                printw("Failed to delete account. (NON-existing account)\n\n");
                continue;
            case DELETE_ACCOUNT_FAIL_PASSWORD :
                clear();
                printw("Failed to delete account. (Wrong password)\n\n");
                continue;
            case EXIT :
                login_flag = 0;
                exit = 1;
                break;
            case INVAILD_CHOICE :
                clear();
                printw("Please input valid choice.\n\n");
                continue;
            case LOGIN_SCCUESS :
                login_flag = 1;
                break;
        }

        if(login_flag == 1){
            char number[5] = {Curr_Num[4], Curr_Num[5], Curr_Num[6], Curr_Num[7], '\0'};
            login_st = search_num(number, Login_Year);
        }

        clear();
       
        while(login_flag){
            switch(run_menu(login_st)){
                case INVAILD_CHOICE :
                    clear();
                    printw("Please input valid choice.\n\n");
                    continue;
                case LOGOUT  :
                    clear();
                    printw("Successfully loged out!\n\n");
                    login_flag = 0;
                    continue;
                case CHANGE_PASSWORD_FAIL :
                    clear();
                    printw("Failed to change password. (Incorrect confirm password)\n\n");
                    continue;
                case CHANGE_PASSWORD_SUCCESS :
                    clear();
                    printw("Successfully changed password!\n\n");
                    continue;
            }
            clear();
        }
        clear();
    }

    endwin();
    Save_Data();
    system("clear");
  
    return 0;
}

int run_account_manage () {
    char command = Account_Manage();

    switch(command) {
        case MENU_LOGIN :
            return login();
        case MENU_NEW :
            return New_Account();
        case MENU_DEL :
            return Delete_Account();
        case MENU_TEMP : 
            Temp_Password();
            break;
        case MENU_QUIT :
            return EXIT;
        
        default : 
            return INVAILD_CHOICE;
    }

    return 0;
}

int run_menu( STUDENT *login_st ) {
    char command = menu();
    int flag = 1;

    switch(command) {
        case MENU_ASSIGN :
            break;
        case MENU_CGPA :
            clear();
            while(flag){
                switch(run_cgpa_menu(login_st)){
                    case MAIN_MENU :
                        flag = 0;
                        clear();
                        continue;
                    case INVAILD_CHOICE :
                        clear();
                        printw("Please input valid choice.\n\n");
                        continue;
                }
                clear();
            }
            return MAIN_MENU;
        case MENU_CHANGE : 
            return Change_Password(login_st);
        case MENU_LOGOUT :
            return LOGOUT;
        default : 
            return INVAILD_CHOICE;
    }
    return 0;
}

int run_cgpa_menu ( STUDENT *login_st ) {
    char command = cgpa_menu();

    switch(command) {
        case CGPA_ADD :
            Add_GPA(login_st);
            break;
        case CGPA_VIEW :
            Print_CGPA(login_st);
            break;
        case CGPA_QUIT :
            return MAIN_MENU;
        default :
            return INVAILD_CHOICE;
    }
    return 0;
}

void Save_Data(){
    FILE* fpoint;

    fpoint = fopen("data.txt", "w");
   
    for(int i = 0; i < TOP->Year_Size; i++){
        STUDENT *current_st = TOP->ST_YEAR[i].ST_NUM;
        
        for(int j = 0; j< TOP->ST_YEAR[i].Num_Size; j++){
            current_st = current_st->link;

            fprintf(fpoint, "%s%s / %s / %d / %d\n", 
                    TOP->ST_YEAR[i].year, 
                    current_st->number, 
                    current_st->password, 
                    current_st->Assign_Size, 
                    current_st->CGPA_Size);
            
            ASSIGN *current_as = current_st->Child_A;

            for(int k = 0; k < current_st->Assign_Size; k++){
                current_as = current_as->link;

                fprintf(fpoint, "%s", current_as->name);
                fprintf(fpoint, "%s", current_as->describe);
                fprintf(fpoint, "%s", current_as->professor);
                fprintf(fpoint, "%d %d\n", current_as->date[0], current_as->date[1]);
            }

            CGPA *current_cg = current_st->Child_C;

            for(int k = 0; k < current_st->CGPA_Size; k++){
                current_cg = current_cg->link;

                fprintf(fpoint, "%d %f\n", current_cg->semester, current_cg->score);
            }
        }
    }

    fclose(fpoint);
}

int search_year (char year[5]) {
    int year_num;

    for(year_num = 0; year_num < TOP->Year_Size; year_num++){
        if(strcmp(TOP->ST_YEAR[year_num].year, year) == 0){
            break; 
        }
    }
    
    return year_num;
}

STUDENT* search_num (char number[5], int year_num) {
    STUDENT *sptr;
    sptr = TOP->ST_YEAR[year_num].ST_NUM;

    for(int i = 0; i < TOP->ST_YEAR[year_num].Num_Size+1; i++){
        if(strcmp(sptr->number, number) == 0){
            break;
        }
        sptr = sptr->link;
    }
    return sptr;
}

void Create_Struct(){
    FILE* fpoint;
    
    fpoint = fopen("data.txt","r");

    TOP = malloc(sizeof(TREE_HEAD));

    TOP->Year_Size = 0;

    read_data(fpoint);

    fclose(fpoint);

}

void read_data (FILE * fpoint) {
    int a_size, c_size, month, date, year_num, semester;
    float score;
    char password[16], year[5], number[5], name[100], describe[100], professor[100];

    while(1){
        if( fscanf(fpoint, "%4s%4s / %s / %d / %d\n", year, number, password, &a_size, &c_size) != 5 ){
            break;
        }
        else{
            create_student(year, number, password, a_size, c_size);
        }

            year_num = search_year(year);
            STUDENT *current_st = search_num(number, year_num);

            ASSIGN *head_as = malloc(sizeof(ASSIGN));
            current_st->Child_A = head_as;

            CGPA *head_cg = malloc(sizeof(CGPA));
            
            current_st->Child_C = head_cg;

        if(a_size != 0){
                ASSIGN *current_as = head_as;
                
            for(int i = 0; i < a_size; i++){
                ASSIGN *new = malloc(sizeof(ASSIGN));
                current_as->link = new;

                fgets(name, 100, fpoint);
                fgets(describe, 100, fpoint);
                fgets(professor, 100, fpoint);
                fscanf(fpoint, "%d %d\n", &month, &date);

                create_assign(new, name, describe, professor, month, date);
                current_as = current_as->link;
                
            }
        }
        
        if(c_size != 0){
            CGPA *current_cg = head_cg;

            for(int i = 0; i < c_size; i++){
                CGPA *new = malloc(sizeof(CGPA));
                current_cg->link = new;

                fscanf(fpoint, "%d %f\n", &semester, &score);

                create_cgpa(new, semester, score);

                current_cg = current_cg->link;
            }
        }
        
    }
}

void create_student (char year[5], char number[5], char password[16], int a_size, int c_size) {
    int year_num;
    
    if(TOP->Year_Size == 0){
        YEAR *year_head = malloc(sizeof(YEAR));
        TOP->ST_YEAR = year_head;

        TOP->Year_Size = 1;

        strcpy(TOP->ST_YEAR[0].year,year);
        
        STUDENT *head = malloc(sizeof(STUDENT));
        TOP->ST_YEAR[0].ST_NUM = head;
        
        STUDENT *new = malloc(sizeof(STUDENT));
        TOP->ST_YEAR[0].ST_NUM->link = new;
        
        TOP->ST_YEAR[0].Num_Size = 1;

        strcpy(new->number, number);
        strcpy(new->password, password);
        new->Assign_Size = a_size;
        new->CGPA_Size = c_size;
    }
    else{
        year_num = search_year(year);

        if(year_num == TOP->Year_Size){
            TOP->Year_Size++;
            TOP->ST_YEAR = realloc(TOP->ST_YEAR, TOP->Year_Size * sizeof(YEAR));
                
            strcpy(TOP->ST_YEAR[year_num].year,year);
        
            STUDENT *head = malloc(sizeof(STUDENT));
            TOP->ST_YEAR[year_num].ST_NUM = head;
    
            STUDENT *new = malloc(sizeof(STUDENT));
            TOP->ST_YEAR[year_num].ST_NUM->link = new;
        
            TOP->ST_YEAR[year_num].Num_Size = 1;

            strcpy(new->number, number);
            strcpy(new->password, password);
            new->Assign_Size = a_size;
            new->CGPA_Size = c_size;
            
        }
        else{
            STUDENT *new = malloc(sizeof(STUDENT));
            STUDENT *sptr = TOP->ST_YEAR[year_num].ST_NUM;

            for(int j = 0; j < TOP->ST_YEAR[year_num].Num_Size; j++){
                sptr = sptr->link;
            }

            sptr->link = new;
        
            TOP->ST_YEAR[year_num].Num_Size++;

            strcpy(new->number, number);
            strcpy(new->password, password);
            new->Assign_Size = a_size;
            new->CGPA_Size = c_size;
        }
        //DONE (for now : 1208)
    }
}

void create_assign 
(ASSIGN *new, char name[100], char describe[100], char professor[100], int month, int date) {
    strcpy(new->name, name);
    strcpy(new->describe, describe);
    strcpy(new->professor, professor);
    new->date[0] = month;
    new->date[1] = date;
}

void create_cgpa (CGPA *new, int semester, float score) {
    new->semester = semester;
    new->score = score;
}

char Account_Manage(){

    printw("1. Login\n");
    printw("2. Create new Account\n");
    printw("3. Delete Account\n");
    printw("4. Get Temporary Password\n");
    printw("5. Exit\n");
    
    noecho();
    return wgetch(stdscr);
}

char menu(){

    printw("1. Assignment Management\n");
    printw("2. CGPA Management\n");
    printw("3. Change Password\n");
    printw("4. Logout\n");
    
    noecho();
    return wgetch(stdscr);
}

void Search_Assign(){
    int Asize;
    char input;

    clear();

    printw("<Assignment Management for %s>\n",Curr_Num);

    Asize = TOP->ST_YEAR[Login_Year].ST_NUM[Login_Num].Assign_Size;

    Sort_Assign();
    Print_Assign(Asize);

    printw("1. New Assignment\n2. Delete Assignment\n3. Return to main menu\n");

    noecho();
    input=wgetch(stdscr);

    switch(input){
        case '1': Add_Assign(); break;
        case '2': Delete_Assign(); break;
        case '3': return; break;
    }

    clear();
}

void Print_Assign(int Asize){
    int k,D_day,thistime,thattime;
    time_t t;
    struct tm *today;
    int i;
    int j;

    i=Login_Year;
    j=Login_Num;
    t = time(NULL);
    today = localtime(&t);
    thistime = mktime(today);
  /*
  	To do...
  */
  
  
}

char cgpa_menu() {

    printw("1. Add new GPA or Change existing GPA\n");
    printw("2. View all\n");
    printw("3. Exit\n");
    
    noecho();

    return wgetch(stdscr);
}

void Add_GPA (STUDENT *login_st) {
    char semester;
    float gpa;
    int gpa_size;

    clear();
    echo();
    printw("Input the semester of GPA : ");
    scanw("%c", &semester);

    gpa_size = login_st->CGPA_Size; 

    if(semester - '0' > (gpa_size + 1) || semester - '0' < 1) {
        printw("You entered wrong number!\n");
        getch();

        return ;
    } 

    printw("Input the GPA : ");
    scanw("%f", &gpa);

    if(gpa < 0 || gpa > 4.31) {
        printw("You entered wrong number!\n");
        getch();

        return ;
    }

    if(semester - '0' <= gpa_size) {
        Cor_GPA(login_st, semester, gpa);
        
        return ;
    }

    login_st->CGPA_Size++;
    gpa_size++;

    CGPA *cptr = login_st->Child_C;


    for(int i = 0; i < gpa_size - 1; i++){
        cptr = cptr->link;
    }

    CGPA *new = malloc(sizeof(CGPA));
    cptr->link = new;

    new->semester = semester - '0';
    new->score = gpa; 
    
    printw("GPA has added successfully!\n");
    getch();

    return ;

}

void Cor_GPA(STUDENT *login_st, char semester, float gpa) {
    CGPA *cptr = login_st->Child_C;
    
    for(int i = 0; i < semester - '0'; i++){
        cptr = cptr->link;
    }

    cptr->score = gpa;

    printw("GPA has changed successfully!\n");
    getch();
    
    return ;

}

void Print_CGPA(STUDENT *login_st) {
    float sum = 0;

    printw("%s\n", login_st->number);

    CGPA *cptr = login_st->Child_C;
    
    clear();

    printw("<CGPA Management for %s>\n", Curr_Num);
    printw("------------------------------\n");
    printw("  Semester  |  GPA  |  CGPA  |  \n");

    for(int i = 0 ; i < login_st->CGPA_Size ; i++) {
        cptr = cptr->link;
        sum += cptr->score;

        printw("------------------------------\n");
        printw("     %d      |  %.2f |  %.2f  |\n", cptr->semester, cptr->score, sum/(i + 1));
    }
    
    printw("------------------------------\n\n");
//    printw("<CGPA Management by Graph for %s>\n", Curr_Num);
//    printw("(x-axis : semester,  y-axis : score)\n");
//    Print_CGPA_Graph();
    
    getch();
}

void Print_CGPA_Graph () {
 	/*
		To do...(Additional)
	*/
}

void Sort_Assign () {
  
  /*
  
  	To do...
  
  */
}

void Add_Assign() {
  //Login_Num,Login_Year
  /*
  	To do...
  
  */
  }

void Delete_Assign() {
  /*
  	To do...
   
  */
}

int New_Account() {
    char year[5], number[5], password[16];
    
    clear();
    printw("Student Number : ");
    echo();
    scanw("%4s%4s\n", year, number);
    printw("Password : ");
    noecho();
    scanw("%s",password);
        
    if(search_year(year) != TOP->Year_Size){
        if(search_num(number, search_year(year)) != NULL){
            return NEW_ACCOUNT_FAIL;
        }
    }

    create_student(year, number, password, 0, 0);
    return NEW_ACCOUNT_SUCCESS;
}

int Delete_Account() {
    int account_year;
    STUDENT *account;

    char year[5], number[5], password[16];
    
    clear();
    printw("Student Number : ");
    echo();
    scanw("%4s%4s\n", year, number);
    printw("Password : ");
    noecho();
    scanw("%s",password);

    account_year = search_year(year);
    account = search_num(number, account_year);

    if(account == NULL){
        return DELETE_ACCOUNT_FAIL_INVALID;
    }
    
    if(strcmp(account->password, password) != 0){
        return DELETE_ACCOUNT_FAIL_PASSWORD;
    }

    STUDENT *sptr = TOP->ST_YEAR[account_year].ST_NUM;

    for(int i = 0; i < TOP->ST_YEAR[account_year].Num_Size; i++){
        if( sptr->link == account ){
            break;
        }
        sptr = sptr->link;
    }
    
    sptr->link = account->link;
    free_student(account);
    
    TOP->ST_YEAR[account_year].Num_Size--;

    return DELETE_ACCOUNT_SUCCESS;
}

void free_student (STUDENT *account) {
    ASSIGN *head_as = account->Child_A;
    CGPA *head_cg = account->Child_C;

    free_assign(head_as);
    free_cgpa(head_cg);
}

void free_assign (ASSIGN *aptr){

    if(aptr->link != NULL){
        free_assign(aptr->link);
    }

    free(aptr);
}

void free_cgpa (CGPA *cptr){

    if(cptr->link != NULL){
        free_cgpa(cptr->link);
    }

    free(cptr);
}

int Change_Password(STUDENT *login_st) {
    char password[16], confirm_password[16];

    clear();

    printw("Enter new password : ");
    noecho();
    scanw("%s",password);
    printw("Confirm new password : ");
    noecho();
    scanw("%s",confirm_password);

    if(strcmp(password, confirm_password) == 0){
        strcpy(login_st->password, password);
        
        return CHANGE_PASSWORD_SUCCESS;
    }
    else{
        return CHANGE_PASSWORD_FAIL;
    }

}

int login() {
    STUDENT *login_st;
    char year[5] = {};
    char number[5] = {};
    char password[16] = {};
    
    clear();
    printw("Student Number : ");
    echo();
    scanw("%4s%4s\n", year, number);
    printw("Password : ");
    noecho();
    scanw("%s",password);

    Login_Year = search_year(year);

    if(Login_Year == TOP->Year_Size){
        return LOGIN_FAILED;
    }

    login_st = search_num(number, Login_Year);
    
    if(login_st == NULL){
        return LOGIN_FAILED;
    }
    
    if(strcmp(login_st->password, password) == 0){
        strcpy(Curr_Num, year);
        strcat(Curr_Num, number);
        return LOGIN_SCCUESS;
    }

    return LOGIN_FAILED;
}

void Temp_Password(){
    char year[5], number[5];
    char temppassword[8];
    float temp;
    int current_yr;
    STUDENT *current_st;

    clear();
    echo();
    srand(time(NULL));

    printw("Student Number : ");
    scanw("%4s%4s",year, number);
        
    current_yr = search_year(year);
    current_st = search_num(number, current_yr);

    if(current_st == NULL){
        printw("Failed to get temporary password. (Wrong student number)\n",current_st->password);
        getch();

        return ;
    }

    printw("Input latest semester's GPA : ");
    scanw("%f", &temp);

    CGPA *cptr = current_st->Child_C;

    for(int i = 0; i < current_st->CGPA_Size; i++){
        cptr = cptr->link;
    }

    if(cptr->score != temp){
        printw("Failed to get temporary password. (Wrong GPA)\n",current_st->password);
        getch();

        return ;
    }

    for(int i = 0; i < 8; i++){
        temppassword[i] = (rand()%93 + 33);
    }

    strcpy(current_st->password,temppassword);

    printw("Temporary Password is <%s>\n",current_st->password);
    getch();

    return ;
}
