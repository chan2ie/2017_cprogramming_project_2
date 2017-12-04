#include "management.h"

int main()
{
  int exit=0;
  int login_flag=0;
  Create_Struct();
  while(!exit)
  { 
    
	/*
		To do...
	*/
	
  }
  endwin();
  Save_Data();
  system("clear");
  return 0;
}
void Save_Data()
{
  int i,j,k;
  int current_year;
  int current_num;
  int current_assign;
  int current_cgpa;
  FILE* fpoint;
  fpoint=fopen("data.txt","w");

	/*
	
		To do...
	
	*/
	


}
void Create_Struct()
{
  int i,j,k;
  int current_year;
  int current_num;
  int current_assign;
  int current_cgpa;
  char* token;
  char temp[350];
  FILE* fpoint;

  /*
  	To do...

  */
}
char Account_Manage()
{
  printw("1. Login\n");
  printw("2. Create new Account\n");
  printw("3. Delete Account\n");
  printw("4. Get Temporary Password\n");
  printw("5. Exit\n");
  return wgetch(stdscr);
}
char menu()
{
  printw("1. Assignment Management\n");
  printw("2. CGPA Management\n");
  printw("3. Change Password\n");
  printw("4. Logout\n");
  return wgetch(stdscr);
}
void Search_Assign()
{
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
  switch(input)
  {
    case '1': Add_Assign(); break;
    case '2': Delete_Assign(); break;
    case '3': return; break;
  }
  clear();
}

void Print_Assign(int Asize)
{
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

char cgpa_menu()
{
  printw("1. Add new GPA or Change existing GPA\n");
  printw("2. View all\n");
  printw("3. Exit\n");
  return wgetch(stdscr);
}
void Search_CGPA() 
{
  int exit;
  Curr_year_index=Login_Year;
  Curr_num_index=Login_Num;
  while(!exit)
  {
    clear();
    switch(cgpa_menu())
    {
      case CGPA_ADD: Add_GPA(); break;
      case CGPA_VIEW: Print_CGPA(); break;
      case CGPA_QUIT: exit = 1; break;
	  default: break;
    }
  }
  exit = 0;
}
void Add_GPA()
{
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

  TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].Child_C = (CGPA*)realloc(TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].Child_C,gpa_size * sizeof(CGPA));

  TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].Child_C[gpa_size - 1].semester = semester - '0';
  TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].Child_C[gpa_size - 1].score = gpa; 

  return ;

}
void Cor_GPA(char semester, float gpa) 
{
  int i;
  TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].Child_C[semester - '0' - 1].score = gpa;
  printw("GPA is changed successfully!\n");
  getch();
  return ;

}
void Print_CGPA() //Additional
{
  int i;
  int tmpSemester;
  float tmpGPA;
  float sum = 0;
  clear();
  printw("<CGPA Management for %s>\n", Curr_Num);
  printw("------------------------------\n");
  printw("  Semester  |  GPA  |  CGPA  |  \n");
  for(i = 0 ; i < TOP -> ST_YEAR[Curr_year_index].ST_NUM[Curr_num_index].CGPA_Size ; i++) 
  {
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
void Print_CGPA_Graph() 
{
 	/*
		To do...(Additional)
	*/
}
void Sort_Assign()
{
  
  /*
  
  	To do...
  
  */
}
void Add_Assign()
{
  //Login_Num,Login_Year
  /*
  	To do...
  
  */
  }
void Delete_Assign()
{
  /*
  	To do...
   
  */
}
void Add_CGPA()
{
  //Login_Num,Login_Year
  
  /*
  	To do...
  */  
  
}
void New_Account()
{
	/*
		To do...
	*/

}
void Delete_Account()
{
    /*
		To do...
    */

}
void Change_Password()
{

	/*
		To do...
	*/

}

int login()
{
  int i,j;
  int year_flag=0;
  int num_flag=0;
  int pass_flag=0;
  char year[5]={};
  char num[5]={};
  for(i=0;i<4;i++)
    year[i]=Curr_Num[i];
  for(i=4;i<8;i++)
    num[i-4]=Curr_Num[i];
  for(i=0;i<TOP->Year_Size;i++)
  {
    if(!strcmp(TOP->ST_YEAR[i].year,year))
    {
      year_flag=1;
      break;
    }
  }
  if(year_flag==1)
  {
    for(j=0;j<TOP->ST_YEAR[i].Num_Size;j++)
    {
      if(!strcmp(TOP->ST_YEAR[i].ST_NUM[j].number,num))
      {
        num_flag=1;
        break;
      }
    }
  }
  if(num_flag==1)
  {
    if(!strcmp(TOP->ST_YEAR[i].ST_NUM[j].password,Curr_Pass))
    {
      pass_flag=1;
      Login_Year=i;//stores logged in user's year
      Login_Num=j;//stores logged in user's number
    }
  }
  if(pass_flag==1)
    return 1;
  return 0;
}

void Temp_Password()
{
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