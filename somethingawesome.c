/* 
*  Benjamin Ly
*  z3374128
*  Mon 18-Drum
*  Genevieve Anne
*  
*  Jun 7
*
*  SOMETHING AWESOME PROJECT
*
*  Program creates 3 daily tasks for the user to complete, while
*  providing basic interactive functions that allows the user to come
*  back and check them off one by one.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SUNDAY 0
#define MONDAY 1
#define TUESDAY 2
#define WEDNESDAY 3
#define THURSDAY 4
#define FRIDAY 5
#define SATURDAY 6

#define TRUE 1
#define FALSE 0

#define MAX_ENTRY_LENGTH 100
#define MAX_DATE_LENGTH 20
#define MAX_DAILY_ENTRIES 3

typedef struct _dateset {
   int day;
   int month;
   int year;
} dateset;

int whatDay(int day, int month, int year);
int fm(int date, int month, int year);
int isLeapYear (int year);
dateset backdateSunday (int today, int day, int month, int year);
int displayTasks(int day, int month, int year, FILE *fp);
int checkTasks (int day, int month, int year, FILE *fp, int taskNumber);
int writeTasks(int day, int month, int year, FILE *fp);

int main(int argc, char * argv[]) {

// Firstly, log (txt) files are created on a weekly basis, with the
// first day being Sunday. This quickly checks for the date of the
// last Sunday and creates and names the file appropriately.
   char str_date[MAX_DATE_LENGTH];
   time_t now = time(NULL);
   struct tm *t = localtime(&now);

   int day;
   int month;
   int year;

// Gets todays date and stores them!
 
   dateset lastSunday;

   strftime(str_date, sizeof(str_date)-1, "%d_%m_%Y", t);
   str_date[MAX_DATE_LENGTH-1] = 0;

   sscanf(str_date, "%d_%d_%d", &day, &month, &year);
   printf("\n\nHello!\nToday is day %d, of month %d in the year %d\n\n\n", 
         day, month, year);

   int today = whatDay(day, month, year);

   lastSunday = backdateSunday(today, day, month, year);

   int fileday = lastSunday.day;
   int filemonth = lastSunday.month;
   int fileyear = lastSunday.year;


// Creates file name in the format WS (week starting) followed by date.
   char filename[MAX_DATE_LENGTH];
   sprintf(filename, "WS%d%d%d.txt", fileday, filemonth, fileyear);

   FILE *fp;

   fp = fopen(filename, "r+");

   if (fp == NULL) {
      
      fp = fopen(filename, "w+");      
      if (fp == NULL) {
         printf("There was an error, file unable to open / create\n");
         return EXIT_FAILURE;
      }
   }

// Opens the file and checks for if today is currently logged!
// If it is, it displays it and allows user to check them off.
// If it isn't, prompts the user to put up three tasks.
   char line[MAX_ENTRY_LENGTH];
   char today_date[MAX_DATE_LENGTH];
   int found = FALSE;
   int menuChoice;
   char lineInput[MAX_ENTRY_LENGTH];
   int sscanfResult;
   int taskNumber;

   sprintf(today_date, "%d_%d_%d", day, month, year);

   while(fgets(line, MAX_ENTRY_LENGTH, fp) != NULL) {
      if(strstr(line, today_date) != NULL) {
         found = TRUE;
      }
   }

   if (found == TRUE) {
      printf("You already have tasks set for today!\n");

      displayTasks(day, month, year, fp);

      while (menuChoice != 0) {

      printf("Would you like to check off some tasks?\n\n"
         "1 - YES\n2 - NO [QUIT]\n\n");

      fgets(lineInput, sizeof(lineInput), stdin);
      sscanfResult = sscanf(lineInput,"%d", &menuChoice);

      if ((sscanfResult == 0) || (sscanfResult == EOF)) {
         printf("\nYou have to enter 1 or 2!\n\n");
         menuChoice = -1;
      }

// Pulls in user input to check out files or exit program.
// Assumes daily entry has been set up.
      
      switch (menuChoice) {
         case 0:
            break;
         case 1:

            while (taskNumber != 0) {

               printf("\nWhich task would you like to check off?\n");

               fgets(lineInput, sizeof(lineInput), stdin);
               sscanfResult = sscanf(lineInput,"%d", &taskNumber);
            
               if ((taskNumber < 1) || (taskNumber > 3)) {
                  taskNumber = -1;
                  printf("Please input 1, 2 or 3!\n");
               } else {
                  break;
               }

            }

            checkTasks(day, month, year, fp, taskNumber);
            menuChoice = 0;
            break;
         case 2:
            printf("Exiting program...\n\n");
            menuChoice = 0;
            break;
         default:
            printf("\nYou have to enter 1 or 2!\n\n");
            menuChoice = -1;
            break;
      }

   }
   } else {
// Daily entry has NOT been set up and will be set up now.
      printf("It doesn't look like you have anything set up for today"
         ".\n\nLet's set some up for today!\n");

      writeTasks(day, month, year, fp);

   }

   fclose(fp);

   return EXIT_SUCCESS;
}

// Function that returns what day of the week a certain date is.
int whatDay(int day, int month, int year) {
   
   int dayOfWeek;
   int YY = year % 100;
   int century = year / 100;

   dayOfWeek = 1.25 * YY + fm(day, month, year) + day - 2 * (century % 4) - 1;
   dayOfWeek = (dayOfWeek % 7);

   return dayOfWeek;
}

// Quick utility function for whatDay.
int fm(int date, int month, int year) {

   int fmonth;
   int leap;

   if (isLeapYear(year) == TRUE) {
      leap = 1;
   } else {
      leap = 0;
   }

   fmonth = 3 + (2 - leap) * ((month + 2) / (2 * month))
         + (5 * month + month / 9) / 2;
 
   fmonth = fmonth % 7;
 
   return fmonth;
} 

// Function to check if a year is a leap year or not.
int isLeapYear (int year) {
int isLeapYear;

if ((year % 4) == 0) {
   if (year % 100 == 0 && year % 400 != 0){
      isLeapYear = FALSE;
   } else {
      isLeapYear = TRUE;
   }
} else isLeapYear = FALSE;

return isLeapYear;
}

// Function that returns the date of the last Sunday.
dateset backdateSunday (int today, int day, int month, int year) {

dateset lastSunday;

   int numDays;

   if (day - today > 0) {
      lastSunday.day = (day - today);
      lastSunday.month = month;
      lastSunday.year = year;
   } else {

      month = month - 1;

      if (month == 0 || month == 1 || month == 3 || month == 5 || 
         month == 7 || month == 8 || month == 10) {
         numDays = 31;
      } else if (month == 4 || month == 6 || month == 9 || 
         month == 11) {
         numDays = 30;
      } else if (isLeapYear(year) == TRUE) {
         numDays = 29;
      } else {
         numDays = 28;
      }

      lastSunday.day = numDays + day - today;
      
      if (month != 0) {
         lastSunday.month = month;
         lastSunday.year = year;
      } else {
         lastSunday.month = 12;
         lastSunday.year = (year - 1);
      }

   }

return lastSunday;
}

// Displays all tasks set for today and also if they are done or not.
int displayTasks(int day, int month, int year, FILE *fp) {
   
   int counter = 0;
   char line[MAX_ENTRY_LENGTH];
   char today_date[MAX_DATE_LENGTH];
   char *lineEdit;

   rewind(fp);

   printf("\nTASKS FOR TODAY\n__________________________________\n\n");   

   sprintf(today_date, "%d_%d_%d", day, month, year);

   while (fgets(line, MAX_ENTRY_LENGTH, fp) != NULL) {  
      if (strstr(line, today_date) != NULL) {
         while(counter<MAX_DAILY_ENTRIES) {       
            fgets(line, MAX_ENTRY_LENGTH, fp);            
            if (line[0] == '!') {
               lineEdit = line;
               lineEdit++;
               printf("%d) %s     DONE!\n", counter + 1, lineEdit);
            } else {        
               printf("%d) %s     NOT DONE!\n", counter + 1, line);
            }  
            counter++;
            if (counter == 3) {
               printf("__________________________________\n");
               return EXIT_SUCCESS;
            }
         }
      }
   }  

   return EXIT_SUCCESS;
}

// Function that checks if a task has been done or not. IF it has, it
// will return that it's been done. If not, it will check the entry
// with an exclamation (!) mark to signal it's done.
int checkTasks (int day, int month, int year, FILE *fp, int taskNumber) {

   int counter = 0;
   char line[MAX_ENTRY_LENGTH];
   char today_date[MAX_DATE_LENGTH];

   rewind(fp); 

   sprintf(today_date, "%d_%d_%d", day, month, year);

   while (fgets(line, MAX_ENTRY_LENGTH, fp) != NULL) {  
      if (strstr(line, today_date) != NULL) {
         while(counter<MAX_DAILY_ENTRIES) {       
            fgets(line, MAX_ENTRY_LENGTH, fp);
            if (counter == taskNumber - 1) {            
               if (line[0] == '!') {
                  printf("Task already checked off!\n");
               } else {        
                  if (line[0] == ' ') {
                     rewind(fp);
                     int secondCounter = 0;
                     while (fgets(line, MAX_ENTRY_LENGTH, fp) != NULL) {  
                        if (strstr(line, today_date) != NULL) {
                           while(secondCounter < MAX_DAILY_ENTRIES) {
                              if (taskNumber == 1) {
                                 fputs("!", fp);
                                 printf("Task is now checked off!\n");
                                 break;
                              } else {
                                 fgets(line, MAX_ENTRY_LENGTH, fp);
                                 if (secondCounter == taskNumber - 2) {
                                    fputs("!", fp);
                                    printf("Task is now checked off!\n");
                                 }
                              }
                              secondCounter++;
                           }
                        }
                     }
                  break;
                  }
               }  
            } 
            counter++;
         }
      }
   }  

   return EXIT_SUCCESS;

}

// Function that takes in 3 user inputted strings and logs them into
// the end of the text file.
int writeTasks(int day, int month, int year, FILE *fp) {

   char today_date[MAX_DATE_LENGTH];
   char firstEntry[MAX_ENTRY_LENGTH];
   char secondEntry[MAX_ENTRY_LENGTH];
   char thirdEntry[MAX_ENTRY_LENGTH];

   sprintf(today_date, "%d_%d_%d", day, month, year);

   fseek(fp, 0, SEEK_END);
   putc('\n', fp);
   fputs(today_date, fp);
   putc('\n', fp);

   printf("\nPlease enter the first task to do today!\n");
   fgets(firstEntry, sizeof(firstEntry), stdin);
   putc(' ', fp);   
   fputs(firstEntry, fp);

   printf("\nPlease enter the second task to do today!\n");
   fgets(secondEntry, sizeof(secondEntry), stdin);
   putc(' ', fp);
   fputs(secondEntry, fp);

   printf("\nPlease enter the final task to do today!\n");
   fgets(thirdEntry, sizeof(thirdEntry), stdin);
   putc(' ', fp);
   fputs(thirdEntry, fp);

   printf("Today's work has been registered!\n");
   
   displayTasks(day, month, year, fp);

   return EXIT_SUCCESS;
}
