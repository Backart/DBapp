#include "securityutils.h"

SecurityUtils::SecurityUtils() {}

QString SecurityUtils::hashPassword(const QString& password) {
    return QString::fromStdString(bcryptcpp::generateHash(password.toStdString()));
}

bool SecurityUtils::verifyPassword(const QString& password, const QString& hash) {
    return bcryptcpp::validatePassword(password.toStdString(), hash.toStdString());
}
