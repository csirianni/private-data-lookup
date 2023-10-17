#include <stdio.h>
#include <sqlite3.h>
#include <iostream>

/*
   Instructions on how to use:
   cd backend/database

   

   Compile the binary:
   g++ database.cpp -l sqlite3 <SELECT or INSERT>
   ./a.out
*/

/* Print out the contents of the table by going password by password. */
static int printData(void *NotUsed, int argc, char **argv, char **azColName)
{
   int i;
   for (i = 0; i < argc; i++)
   {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

/* Executes the main part of the function by inserting or selecting the SQL Query */
int main(int argc, char* argv[]) {
   std::cout << argv[1] << std::endl;
   sqlite3 *db;
   char *err = 0;
   int res;

   /* Open database */
   res = sqlite3_open("breachedPasswords.db", &db);

   if( res ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }

   /* Create SQL Table named PASSWORDS with column PASSWORD 
      This only needs to be initialized once. */ 
   // const char* sql = "CREATE TABLE PASSWORDS("
   //             "PASSWORD   TEXT  PRIMARY KEY  NOT NULL); ";

   /* Insert two passwords (yourmoms.com and badpassword12) into the table */
   // const char *sql = "INSERT INTO PASSWORDS (PASSWORD)"
   //                   "VALUES ('yourmomsss.com')"
   //                   "INSERT INTO PASSWORDS (PASSWORD)"
   //                   "VALUES ('badpassword123');";

   /* Get all passwords SQL query */
   const char* sql;
   const char* command = argv[1];
   if (strcmp(command,"INSERT") == 0) { // insert the password
      const char* password = argv[2];
      sql = "INSERT INTO PASSWORDS (PASSWORD)"
            "VALUES ('mymomscool13');";
   } else if (strcmp(command, "SELECT") == 0) { // select all passwords
      sql = "SELECT * from PASSWORDS";
   } else {
      return 1;
   }

   /* Execute SQL statement */
   res = sqlite3_exec(db, sql, printData, 0, &err);

   /* Determines if the response has any errors to print out to terminal.*/
   if (res != SQLITE_OK)
   {
      fprintf(stderr, "SQL error: %s\n", err);
      sqlite3_free(err);
   }
   else
   {
      fprintf(stdout, "SQL successful.\n");
   }

   sqlite3_close(db); // closes the database
   return 0; 
}