#ifndef ERRORMESSAGES_H
#define ERRORMESSAGES_H

#include <QMessageBox>
#include <QString>
#include <QSqlError>
#include <QMap>

class ErrorMessages
{
public:
    ErrorMessages();

    enum ErrorCode {
        ERROR_100 = 100,
        ERROR_101 = 101,
        ERROR_200 = 200,
        ERROR_201 = 201,
        ERROR_204 = 204,
        ERROR_301 = 301,
        ERROR_302 = 302,
        ERROR_304 = 304,
        ERROR_400 = 400,
        ERROR_401 = 401,
        ERROR_403 = 403,
        ERROR_404 = 404,
        ERROR_500 = 500,
        ERROR_502 = 502,
        ERROR_503 = 503
    };

    static void showMessage(ErrorCode code, const QString& details = "");

};

#endif // ERRORMESSAGES_H
