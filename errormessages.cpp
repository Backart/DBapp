#include "errormessages.h"

ErrorMessages::ErrorMessages() {}

void ErrorMessages::showWarning(ErrorCode code, const QString& details) {
    QString message;
    switch (code) {
    case ERROR_400:
        message = "Bad Request. " + details;
        break;
    case ERROR_401:
        message = "Unauthorized access. " + details;
        break;
    default:
        message = "Unknown warning. " + details;
        break;
    }
    QMessageBox::warning(nullptr, "Warning", message);
}

void ErrorMessages::showInfo(const QString& title, const QString& message) {
    QMessageBox::information(nullptr, title, message);
}

void ErrorMessages::showCritical(ErrorCode code, const QString& details) {
    QString message;
    switch (code) {
    case ERROR_500:
        message = "Internal Server Error. " + details;
        break;
    default:
        message = "Unknown critical error. " + details;
        break;
    }
    QMessageBox::critical(nullptr, "Critical Error", message);
}
