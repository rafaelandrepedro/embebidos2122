#ifndef DATABASE_H
#define DATABASE_H
#include "sqlite3.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
}

class Database{
	private:
		sqlite3 *db;
		char *zErrMsg;
		const char* data;
	public:
		Database(const char* name){
			init(name);
			zErrMsg = (char *)0;
			data = "Callback function called";
		}
		
		~Database(){
			sqlite3_close(db);
		}
		
		int init(const char* name){
			int rc;
			/* Open database */
			rc = sqlite3_open(name, &db);
			
			if( rc ) {
				fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
				return 0;
			} 
			else {
				fprintf(stderr, "Opened database successfully\n");
				return 1;
			}
		}
		
		void quarry(char *sql){
			int rc;
			/* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
			   
			if( rc != SQLITE_OK ) {
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			} 
			else {
				fprintf(stdout, "Operation done successfully\n");
			}
		}
	
};

#endif

