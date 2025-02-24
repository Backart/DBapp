#ifndef ERRORMESSAGES_H
#define ERRORMESSAGES_H

#include <QMessageBox>
#include <QString>
#include <QSqlError>

class ErrorMessages
{
public:
    ErrorMessages();

    // Перерахування типів помилок
    enum ErrorCode {
        ERROR_400 = 400,  // Невірний запит
        ERROR_401 = 401,  // Неавторизовано
        ERROR_500 = 500   // Внутрішня помилка сервера
    };

    // Статичні методи для відображення повідомлень
    static void showWarning(ErrorCode code, const QString& details = "");
    static void showInfo(const QString& title, const QString& message);
    static void showCritical(ErrorCode code, const QString& details = "");
};

#endif // ERRORMESSAGES_H
