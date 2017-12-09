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
void Del_GPA ();
void Cor_GPA (char semester,float gpa);
void Create_Struct ();
void Search_Assign ();
void Search_CGPA ();
void Print_Assign( int);
void Print_CGPA ();
void Print_CGPA_Graph ();
void New_Account ();
void Delete_Account ();
void Change_Password ();
void rank ();
void Sort_Assign ();
void Add_Assign ();
void Delete_Assign ();
void Temp_Password ();
int login ();
void Temp_Password ();
char Account_Manage ();
void read_data (FILE * fpoint);
void create_student 
    (char year[5], char number[5], char password[16], int a_size, int c_size);
void create_assign 
    (ASSIGN *current_as, char name[100], char describe[100], char professor[100], int month, int date);
void create_cgpa (CGPA *new, int semester, float score);
int search_year(char year[5]);
STUDENT*  search_num(char number[5], int year_num);

int main(){
    int exit=0;
    int login_flag=0;

    initscr();

    Create_Struct();

    while(!exit){ 
    
	/*
		To do...
	*/
    exit = 1;
	
    }

    endwin();
    Save_Data();
    system("clear");
  
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

    for(int i = 0; i < TOP->ST_YEAR[year_num].Num_Size; i++){
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

    //TODO
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

    return wgetch(stdscr);
}

char menu(){

    printw("1. Assignment Management\n");
    printw("2. CGPA Management\n");
    printw("3. Change Password\n");
    printw("4. Logout\n");
    
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
    
    return wgetch(stdscr);
}

void Search_CGPA () {
    int exit;
    Curr_year_index=Login_Year;
    Curr_num_index=Login_Num;
    while(!exit){

        clear();

        switch(cgpa_menu()){
            case CGPA_ADD: Add_GPA(); break;
            case CGPA_VIEW: Print_CGPA(); break;
            case CGPA_QUIT: exit = 1; break;

	        default: break;
        }
    }

    exit = 0;
}

void Add_GPA () {
    char semester;
    float gpa;
    int gpa_size;
    clear();
    echo();
    printw("Input the semester of GPA : ");
    scanw("%c", &semester);

    gpa_size = TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].CGPA_Size;  

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
        Cor_GPA(semester, gpa);
        
        return ;
    }

    TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].CGPA_Size++;
    gpa_size++;
    
    TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].Child_C
        = (CGPA*)realloc(TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].Child_C,gpa_size * sizeof(CGPA));

    TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].Child_C[gpa_size - 1].semester = semester - '0';
    TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].Child_C[gpa_size - 1].score = gpa; 

    return ;

}

void Cor_GPA(char semester, float gpa) {
    int i;
    
    TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].Child_C[semester - '0' - 1].score = gpa;
  
    printw("GPA is changed successfully!\n");
    getch();
    
    return ;

}

void Print_CGPA() {
    int i;
    int tmpSemester;
    float tmpGPA;
    float sum = 0;
    
    clear();
    printw("<CGPA Management for %s>\n", Curr_Num);
    printw("------------------------------\n");
    printw("  Semester  |  GPA  |  CGPA  |  \n");
    for(i = 0 ; i < TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].CGPA_Size ; i++) {
    /*
		To do...
    */
        printw("------------------------------\n");
        printw("     %d      |  %.2f |  %.2f  |\n", tmpSemester, tmpGPA, sum/(i + 1));
    }
    
    printw("------------------------------\n\n");
    printw("<CGPA Management by Graph for %s>\n", Curr_Num);
    printw("(x-axis : semester,  y-axis : score)\n");
    Print_CGPA_Graph();
    
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

void New_Account() {
	/*
		To do...
	*/

}

void Delete_Account() {
    /*
		To do...
    */

}

void Change_Password() {

	/*
		To do...
	*/

}

int login() {
    int i,j;
    int year_flag=0;
    int num_flag=0;
    int pass_flag=0;
    char year[5]={};
    char num[5]={};
    
    for(i=0;i<4;i++){
        year[i]=Curr_Num[i];
    }

    for(i=4;i<8;i++){
        num[i-4]=Curr_Num[i];
    }

    for(i=0;i<TOP->Year_Size;i++){
        if(!strcmp(TOP->ST_YEAR[i].year,year)){
            year_flag=1;
            break;
        }
    }

    if(year_flag==1){
        for(j=0;j<TOP->ST_YEAR[i].Num_Size;j++){
            if(!strcmp(TOP->ST_YEAR[i].ST_NUM[j].number,num)){
                num_flag=1;
                break;
            }
        }
    }

    if(num_flag==1){
        if(!strcmp(TOP->ST_YEAR[i].ST_NUM[j].password,Curr_Pass)){
            pass_flag=1;
            Login_Year=i;//stores logged in user's year
            Login_Num=j;//stores logged in user's number
        }
    }

    if(pass_flag==1){
        return 1;
    }

    return 0;
}

void Temp_Password(){
    int flag1=0;
    int i, j, k;
    char num[5]={};
    char temppassword[8];
    float temp;

    clear();
    echo();
    srand(time(NULL));
    printw("Student Number:");
    scanw("%s",Curr_Num);
	/*
		To do...
	*/
}
