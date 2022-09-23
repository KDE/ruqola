/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"

#include <QJsonObject>
#include <QDebug>
class LIBRUQOLACORE_EXPORT ServicePassword
{
public:
    ServicePassword();
    ~ServicePassword();
    Q_REQUIRED_RESULT bool operator==(const ServicePassword &other) const;
    void parseService(const QJsonObject &replyObject);

    Q_REQUIRED_RESULT bool email2faEnabled() const;
    void setEmail2faEnabled(bool email2faEnabled);

    Q_REQUIRED_RESULT QString password() const;
    void setPassword(const QString &password);

    Q_REQUIRED_RESULT bool totp() const;
    void setTotp(bool totp);

private:
    QString mPassword;
    bool mEmail2faEnabled = false;
    bool mTotp = false;
};

Q_DECLARE_METATYPE(ServicePassword)
Q_DECLARE_TYPEINFO(ServicePassword, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ServicePassword &t);
