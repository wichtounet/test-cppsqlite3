#include <ctime>
#include <iostream>

#include "CppSQLite3.h"

int main(int, char**){
    try {
        /* Remove the database (THIS WONT BE IN A FINAL APPLICATION) */

        remove("test.db");

        /* Create the database object */

        CppSQLite3DB db;

        /* Print the SQLite version */

        std::cout << "SQLite Version: " << db.SQLiteVersion() << std::endl;

        /* Open (connect) the database */

        db.open("test.db");

        /* Create a table */

        db.execDML("create table emp(empno int, empname char(20));");

        /* Perform insertions */

        int nRows = db.execDML("insert into emp values (7, 'David Beckham');");
        std::cout << nRows << " rows inserted" << std::endl;

        /* Perform updates */

        nRows = db.execDML( "update emp set empname = 'Christiano Ronaldo' where empno = 7;");
        std::cout << nRows << " rows updated" << std::endl;

        /* Perform deletions */

        nRows = db.execDML("delete from emp where empno = 7;");
        std::cout << nRows << " rows deleted" << std::endl;

        /* Execute a scalar operation (only retuns one value) */

        std::cout << db.execScalar("select count(*) from emp;") << " rows in emp table ";

        /* Insert and get the ID of the inserted row (auto increment primary key */

        std::cout << std::endl << "Auto increment test" << std::endl;
        db.execDML("drop table emp;");
        db.execDML( "create table emp(empno integer primary key, empname char(20));");

        for (int i = 0; i < 5; i++){
            char buf[128];
            sprintf(buf, "insert into emp (empname) values ('Empname%06d');", i+1);
            db.execDML(buf);
            std::cout << " primary key: " << db.lastRowId() << std::endl;
        }

        /* Execute a SELECT and display the results */

        std::cout << std::endl << "Select statement test" << std::endl;
        CppSQLite3Query q = db.execQuery("select * from emp order by 1;");

        for (int field = 0; field < q.numFields(); field++){
            std::cout << q.fieldName(field) << "|";
        }

        std::cout << std::endl;

        while (!q.eof()){
            std::cout << q.fieldValue(0) << "|"
                      << q.fieldValue(1) << "|" << std::endl;
            q.nextRow();
        }

        /* The wrapper has some utility functions to create queries with quotes */

        CppSQLite3Buffer bufSQL;
        bufSQL.format("insert into emp (empname) values (%Q);", "He's bad");
        std::string query_result(bufSQL);
        std::cout << query_result << std::endl;
    } catch (CppSQLite3Exception& e){
        std::cerr << e.errorCode() << ":" << e.errorMessage() << std::endl;
    }

    return 0;
}
