#include "repository.h"

#include <qsqldatabase.h>
#include "QSqlQuery"

repository::repository()
{

}

std::vector<QString> repository::getNames(){
    std::vector<QString> tests;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("MindTesterDbNew2.sqlite");
    if (!db.open())
    {
    return tests;
    }
    QSqlQuery testquery("SELECT * FROM TESTS");
    while(testquery.next()){
        tests.push_back(testquery.value(1).toString());}
    db.close();
    return tests;
}

std::vector<QString> repository::getTasks(int id){
    std::vector<QString> tasks;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("MindTesterDbNew2.sqlite");
    if (!db.open())
    {
    return tasks;
    }
    QString st = "SELECT * FROM TASKS WHERE TEST_ID == ";
    st.append(QString::number(id));
    QSqlQuery titlequery(st);
    while(titlequery.next()){
        QString taskTitle = titlequery.value(1).toString();
        tasks.push_back(taskTitle);

        QString st1 = "SELECT * FROM VARIANTS WHERE TASK_ID == ";
        st1.append(QString::number(titlequery.value(0).toInt()));

        QSqlQuery variantquery(st1);
        while(variantquery.next()){
            QString varcorrect = variantquery.value(2).toString();
            QString varTitle = variantquery.value(1).toString();

            if(varcorrect.toInt() == 1){
                varTitle.append("+");
            }
            tasks.push_back(varTitle);
        }
        tasks.push_back(" ");
    }
    db.close();
    return tasks;
}

void repository::generateDBIfNotExist()
{
     QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
     db.setDatabaseName("MindTesterDbNew2.sqlite");
     if (!db.open())
     {
     return;
     }
     QSqlQuery *query = new QSqlQuery();
     QString qstr = "CREATE TABLE IF NOT EXISTS TESTS ("
     "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
     "TITLE VARCHAR(255) "
     ");";
     if(query->exec(qstr))
     {

        qstr = "INSERT INTO TESTS (TITLE) VALUES ('Kpi test');";
        query->exec(qstr);
        qstr = "INSERT INTO TESTS (TITLE) VALUES ('Morshynska test');";
        query->exec(qstr);

     }

     qstr = "CREATE TABLE IF NOT EXISTS TASKS ("
     "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
     "TITLE VARCHAR(255), "
     "TEST_ID INTEGER, "
     "FOREIGN KEY(TEST_ID) REFERENCES TESTS(ID)"
     ");";
     if(query->exec(qstr))
     {

         qstr = "INSERT INTO TASKS (TITLE, TEST_ID) VALUES ('Best faculty:', 1);";
         query->exec(qstr);
         qstr = "INSERT INTO TASKS (TITLE, TEST_ID) VALUES ('Dest ob$chaga:', 1);";
         query->exec(qstr);
         qstr = "INSERT INTO TASKS (TITLE, TEST_ID) VALUES ('How much water do u need?', 2);";
         query->exec(qstr);


     }

     qstr = "CREATE TABLE IF NOT EXISTS VARIANTS ("
     "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
     "TITLE VARCHAR(255), "
     "ISCORRECT INTEGER, "
     "TASK_ID INTEGER, "
     "FOREIGN KEY(TASK_ID) REFERENCES TASKS(ID)"
     ");";
     if(query->exec(qstr))
     {

         qstr = "INSERT INTO VARIANTS (TITLE, ISCORRECT, TASK_ID) VALUES ('TEF', 0, 1);";
         query->exec(qstr);
         qstr = "INSERT INTO VARIANTS (TITLE, ISCORRECT, TASK_ID) VALUES ('FMM', 0, 1);";
         query->exec(qstr);
         qstr = "INSERT INTO VARIANTS (TITLE, ISCORRECT, TASK_ID) VALUES ('FICT', 1, 1);";
         query->exec(qstr);
         qstr = "INSERT INTO VARIANTS (TITLE, ISCORRECT, TASK_ID) VALUES ('HTF', 0, 1);";
         query->exec(qstr);
         qstr = "INSERT INTO VARIANTS (TITLE, ISCORRECT, TASK_ID) VALUES ('6', 1, 2);";
         query->exec(qstr);
         qstr = "INSERT INTO VARIANTS (TITLE, ISCORRECT, TASK_ID) VALUES ('16', 0, 2);";
         query->exec(qstr);
         qstr = "INSERT INTO VARIANTS (TITLE, ISCORRECT, TASK_ID) VALUES ('1', 0, 2);";
         query->exec(qstr);
         qstr = "INSERT INTO VARIANTS (TITLE, ISCORRECT, TASK_ID) VALUES ('0,5', 0, 3);";
         query->exec(qstr);
         qstr = "INSERT INTO VARIANTS (TITLE, ISCORRECT, TASK_ID) VALUES ('10', 0, 3);";
         query->exec(qstr);
         qstr = "INSERT INTO VARIANTS (TITLE, ISCORRECT, TASK_ID) VALUES ('1,5', 1, 3);";
         query->exec(qstr);


     }

     db.close();

}
