#include "management.h"

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
    free_struct();

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

            clear();
            flag = 1;

            while(flag){
                switch(run_assign_menu(login_st)){
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

        case MENU_CGPA :

            clear();
            flag = 1;

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

                fprintf(fpoint, "%s\n", current_as->name);
                fprintf(fpoint, "%s\n", current_as->describe);
                fprintf(fpoint, "%s\n", current_as->professor);
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

int search_year (char year[]) {
    int year_num;

    for(year_num = 0; year_num < TOP->Year_Size; year_num++){
        if(strcmp(TOP->ST_YEAR[year_num].year, year) == 0){
            break;
        }
    }

    return year_num;
}

STUDENT* search_num (char number[], int year_num) {
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
        if( fscanf(fpoint, "%4s%4s / %s / %d / %d\n",
            year, number, password, &a_size, &c_size) != 5 ){
            break;
        }
        else{
            create_student(year, number, password, a_size, c_size);
        }

            year_num = search_year(year);
            STUDENT *current_st = search_num(number, year_num);

        if(a_size != 0){
                ASSIGN *current_as = current_st->Child_A;

            for(int i = 0; i < a_size; i++){
                ASSIGN *new = malloc(sizeof(ASSIGN));
                current_as->link = new;

                fgets(name, 100, fpoint);
                fgets(describe, 100, fpoint);
                fgets(professor, 100, fpoint);
                fscanf(fpoint, "%d %d\n", &month, &date);

                name[strlen(name) - 1] = '\0';
                describe[strlen(describe) - 1] = '\0';
                professor[strlen(professor) - 1] = '\0';

                create_assign(new, name, describe, professor, month, date);
                current_as = current_as->link;

            }
        }

        if(c_size != 0){
            CGPA *current_cg = current_st->Child_C;

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
    STUDENT *new = malloc(sizeof(STUDENT));

    if(TOP->Year_Size == 0){
        YEAR *year_head = malloc(sizeof(YEAR));
        TOP->ST_YEAR = year_head;

        TOP->Year_Size = 1;

        strcpy(TOP->ST_YEAR[0].year,year);

        STUDENT *head = malloc(sizeof(STUDENT));
        head->link = new;
        TOP->ST_YEAR[0].ST_NUM = head;

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

            TOP->ST_YEAR[year_num].ST_NUM->link = new;

            TOP->ST_YEAR[year_num].Num_Size = 1;

            strcpy(new->number, number);
            strcpy(new->password, password);
            new->Assign_Size = a_size;
            new->CGPA_Size = c_size;

        }
        else{
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
    }
    ASSIGN *head_as = malloc(sizeof(ASSIGN));
    new->Child_A = head_as;
    head_as->link = NULL;

    CGPA *head_cg = malloc(sizeof(CGPA));
    new->Child_C = head_cg;
    head_cg->link = NULL;

    new->link = NULL;
    //DONE (for now : 1208)
}

void create_assign
(ASSIGN *new, char name[100], char describe[100], char professor[100], int month, int date) {
    strcpy(new->name, name);
    strcpy(new->describe, describe);
    strcpy(new->professor, professor);
    new->date[0] = month;
    new->date[1] = date;

    new->link = NULL;
}

void create_cgpa (CGPA *new, int semester, float score) {
    new->semester = semester;
    new->score = score;

    new->link = NULL;
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

int run_assign_menu (STUDENT *login_st){
    char command;

    printw("<Assignment Management for %s>\n\n",Curr_Num);

    Sort_Assign(login_st);
    Print_Assign(login_st);

    printw("1. New Assignment\n");
    printw("2. Delete Assignment\n");
    printw("3. Return to main menu\n");

    noecho();
    command = wgetch(stdscr);

    switch(command){
        case '1':
            clear();
            Add_Assign(login_st);
            break;
        case '2':
            clear();
            Delete_Assign(login_st);
            break;
        case '3':
            return MAIN_MENU;

        default :
            return INVAILD_CHOICE;
    }

    clear();
}

void Print_Assign(STUDENT *login_st){
    int differ;
    time_t t, d;
    struct tm d_day, *today;

    ASSIGN *aptr = login_st->Child_A;

    t = time(NULL);
    today = localtime(&t);

    for(int i = 0; i < login_st->Assign_Size; i++){
        aptr = aptr->link;

        printw("<%d> Name\t\t : %s\n", i + 1, aptr->name);
        printw("    Describe\t\t : %s\n", aptr->describe);
        printw("    Professor\t\t : %s\n", aptr->professor);
        printw("    Due : %d/%d\n", aptr->date[0], aptr->date[1]);

        d_day.tm_year = today->tm_year;
        d_day.tm_mon = aptr->date[0] - 1;
        d_day.tm_mday = aptr->date[1];
        d_day.tm_hour = today->tm_hour;
        d_day.tm_min = today->tm_min;
        d_day.tm_sec = today->tm_sec;
        d_day.tm_isdst = 0;

        d = mktime(&d_day);

        differ = (int)(t - d);

        if(differ > 0){
            printw("    D-day over!!!\n\n");
        }
        else if(differ == 0){
            printw("    D-day\t\t : TODAY!\n\n ");
        }
        else if(differ < 0){
            printw("    D-day\t\t : %d\n\n", (int)(differ / 86400));
        }

    }

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
    new->link = NULL;

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

    getch();
}

void Sort_Assign (STUDENT *login_st) {
    ASSIGN *check, *check_bf, *bef, *af;


    for(int i = 1; i < login_st->Assign_Size; i++){
        check = login_st->Child_A->link;
        check_bf = login_st->Child_A;

        for(int j = 0; j < i; j++){
            check = check->link;
            check_bf = check_bf->link;
        }

        for(int j = 0; j < i; j++){
            bef = login_st->Child_A->link;

            for(int k = 0; k < j; k++){
                bef = bef->link;
            }

            if(j == 0){
                if(100*check->date[0] + check->date[1] < 100*bef->date[0] + bef->date[1]){
                    login_st->Child_A->link = check;
                    check_bf->link = check->link;
                    check->link = bef;

                    j = i;
                    break;
                }
            }
            else ;

            if(j == (i - 1)) ;
            else{
                if(100*check->date[0] + check->date[1]
                    >= 100*bef->date[0] + bef->date[1]
                    && 100*check->date[0] + check->date[1]
                    < 100*bef->link->date[0] + bef->link->date[1]){
                    af = bef->link;
                    bef->link = check;
                    check_bf->link = check->link;
                    check->link = af;

                    j = i;
                    break;
                }
            }
        }
    }

}

void Add_Assign(STUDENT *login_st) {
    char name[100], describe[100], professor[100];
    int month, date;

    echo();
    printw("Enter the name of new assignment :\n");

    printw("");
    getstr(name);
    printw("Enter the description of new assignment :\n");
    getstr(describe);
    printw("Enter the professor of new assignment :\n");
    getstr(professor);
    printw("Enter the due month of new assignment : ");
    scanw("%d\n", &month);
    printw("Enter the due date of new assignment : ");
    scanw("%d\n", &date);

    ASSIGN *aptr = login_st->Child_A;

    for(int i = 0; i < login_st->Assign_Size; i++){
        aptr = aptr->link;
    }

    ASSIGN *new = malloc(sizeof(ASSIGN));
    aptr->link = new;

    create_assign(new, name, describe, professor, month, date);

    login_st->Assign_Size++;

    printw("\nNew Assignment is successfully added!\n");
    getch();
}

void Delete_Assign(STUDENT *login_st) {
    char name[100];

    echo();
    printw("Input the name of assignment you want to delete :\n");
    getstr(name);

    ASSIGN *aptr = login_st->Child_A->link;
    ASSIGN *before = login_st->Child_A;

    for(int i = 0; i < login_st->Assign_Size; i++){
        if(strcmp(aptr->name, name) == 0){
            break;
        }
        aptr = aptr->link;
        before = before->link;
    }

    if(aptr == NULL){
        printw("\nYou don't have assignment named '%s'!\n", name);
        getch();
        return ;
    }
    else if(strcmp(aptr->name, name) != 0){
        printw("\nYou don't have assignment named '%s'!\n", name);
        getch();
        return ;
    }
    else{
        before->link = aptr->link;
        free(aptr);

        login_st->Assign_Size--;
    }
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
            printw("this account\n");
            break;
        }
        sptr = sptr->link;
    }

    sptr->link = account->link;
    free_student(account);

    TOP->ST_YEAR[account_year].Num_Size--;

    return DELETE_ACCOUNT_SUCCESS;
}

void free_struct () {
    for(int i = 0; i < TOP->Year_Size; i++){
        free_year(TOP->ST_YEAR[i].ST_NUM->link);
        free(TOP->ST_YEAR[i].ST_NUM);
    }
    if(TOP->Year_Size != 0){
        free(TOP->ST_YEAR);
    }
    free(TOP);
}

void free_year (STUDENT* sptr) {
    
    if(sptr->link != NULL){
        free_year(sptr->link);
    }
    free_student(sptr);
    free(sptr);
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
        printw("Failed to get temporary password. (Wrong student number)\n");
        getch();

        return ;
    }

    if(current_st->Child_C->link == NULL){
        printw("Failed to get temporary password. (No CGPA Information)\n");
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
