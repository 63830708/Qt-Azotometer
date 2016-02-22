#ifndef WBGLOBAL_H
#define WBGLOBAL_H

#include "qtankgui.h"

#define pline() qDebug() << __FILE__ << __func__ << __LINE__
#define perr(var, cond) if(var == cond) pline() << hex << cond

extern QSqlDatabase managerDB;

extern QString gUserName;
extern QString gPassword;

QSqlDatabase newDatabaseConn();

void setDatabaseName(QSqlDatabase &db, QString dbName);

void openDatabase(QSqlDatabase &db);

void closeDatabase(QSqlDatabase &db);

void moveCenter(QWidget* w);

void moveRight(QWidget* w);

#endif // WBGLOBAL_H
