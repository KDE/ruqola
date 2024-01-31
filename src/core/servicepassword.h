/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"

#include <QDebug>
#include <QJsonObject>
class LIBRUQOLACORE_EXPORT ServicePassword
{
public:
    ServicePassword();
    ~ServicePassword();
    [[nodiscard]] bool operator==(const ServicePassword &other) const;
    void parseService(const QJsonObject &replyObject);

    [[nodiscard]] bool email2faEnabled() const;
    void setEmail2faEnabled(bool email2faEnabled);

    [[nodiscard]] QString password() const;
    void setPassword(const QString &password);

    [[nodiscard]] bool totp() const;
    void setTotp(bool totp);

private:
    QString mPassword;
    bool mEmail2faEnabled = false;
    bool mTotp = false;
};

Q_DECLARE_METATYPE(ServicePassword)
Q_DECLARE_TYPEINFO(ServicePassword, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ServicePassword &t);
