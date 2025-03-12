#include "errormessages.h"

ErrorMessages::ErrorMessages() {}

void ErrorMessages::showMessage(ErrorCode code, const QString& details) {
    static const QMap<ErrorCode, QString> messages = {
        {ERROR_100, "Continue"},
        {ERROR_101, "Switching Protocols"},
        {ERROR_200, "OK"},
        {ERROR_201, "Created"},
        {ERROR_204, "No Content"},
        {ERROR_301, "Moved Permanently"},
        {ERROR_302, "Found"},
        {ERROR_304, "Not Modified"},
        {ERROR_400, "Bad Request"},
        {ERROR_401, "Unauthorized"},
        {ERROR_403, "Forbidden"},
        {ERROR_404, "Not Found"},
        {ERROR_500, "Internal Server Error"},
        {ERROR_502, "Bad Gateway"},
        {ERROR_503, "Service Unavailable"}
    };

    QString message = messages.contains(code) ? messages[code] : "Unknown error";
    message += ". " + details;

    QMessageBox::Icon icon;
    if (code >= ERROR_500) {
        icon = QMessageBox::Critical;
    } else if (code >= ERROR_400) {
        icon = QMessageBox::Warning;
    } else {
        icon = QMessageBox::Information;
    }

    QMessageBox msgBox;
    msgBox.setIcon(icon);
    msgBox.setWindowTitle("Message");
    msgBox.setText(message);
    msgBox.exec();
}
