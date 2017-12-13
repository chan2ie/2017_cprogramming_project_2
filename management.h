#ifndef MANAGEMENT_H
#define MANAGEMENT_H
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
void Print_Assign ();
void Print_CGPA ();
int New_Account ();
int Delete_Account ();
int Change_Password ();
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
int run_assign_menu ();
void free_student ();
void free_assign ();
void free_cgpa ();
#endif
