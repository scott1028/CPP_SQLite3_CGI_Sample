#include <stdio.h>
#include <sqlite3.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// 如果有回傳值(例如 select 由此輸出)
static int callback_json(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	//cout << "{";
	for(i=0; i<argc; i++){
		//printf("\"%s\" : %s, ", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	//cout << "}, ";
	return 0;
}

// 執行 SQL 的副程式
void do_sql_json(string sql, int rc, char *zErrMsg, sqlite3 *db){
	rc = sqlite3_exec(db, sql.c_str(), callback_json, 0, &zErrMsg);
        if( rc != SQLITE_OK ){
			//fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
        }else{
			//fprintf(stdout, "SQL Command successfully\n");
        }
}

int main(int argc, char* argv[])
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	string sql;
	char i2s[4096];		// 用來數字轉字串的暫存

	rc = sqlite3_open("test.db", &db);

	if( rc ){
		//fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return 0;
	}
	else{
		//fprintf(stderr, "Opened database successfully\n");
	}
	
	sql = "CREATE TABLE COMPANY("\
		"ID				INTEGER	NOT NULL,"\
		"NAME			TEXT    NOT NULL,"\
		"AGE            INT     NOT NULL,"\
		"ADDRESS        CHAR(50),"\
		"SALARY         REAL,"\
		"PRIMARY KEY (\"ID\")"\
	");";
	do_sql(sql, rc, zErrMsg, db);	// Create
	
	for(int i=0;i<10;i++){
		sprintf(i2s, "INSERT INTO COMPANY (NAME,AGE,ADDRESS,SALARY) VALUES ('Paul', %d, 'California', 20000.00 );" ,i);
		sql=string(i2s);
		do_sql(sql, rc, zErrMsg, db);	// Create
	}
	
	sql = "SELECT * from COMPANY";
	cout << "Content-Type: text/json" << endl << endl;
	do_sql_json(sql, rc, zErrMsg, db);	// Read

	// Post Data Handle (與 Get Handle 不能共存)
	int contentLength = atoi( getenv( "CONTENT_LENGTH" ) );
	char *postString=new char[contentLength+1];
	cin.read( postString, contentLength );
	postString[contentLength] = '\0';		// set EOF point by yourself...
	cout << "[";
	cout << "{ \"post\" : \"" << postString << "\" }";
	cout << ",";
	cout << "{ \"len\" : \"" << contentLength << "\" }";
	cout << "]" << endl;
	
	// Get Data Handle (與 Post Handle 不能共存)
	//cout << "[{ \"get\" : \"" << getenv("QUERY_STRING") << "\" }]";
	
	sqlite3_close(db);
	return 0;
}

