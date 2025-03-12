#ifndef SECURITYUTILS_H
#define SECURITYUTILS_H

#include <QString>
#include "bcryptcpp.h"

class SecurityUtils
{
public:
    SecurityUtils();

    static QString hashPassword(const QString& password);
    static bool verifyPassword(const QString& password, const QString& hash);

};

#endif // SECURITYUTILS_H
