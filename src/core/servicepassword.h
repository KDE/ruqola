/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QMetaType>
class QJsonObject;
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

QT_DECL_METATYPE_EXTERN_TAGGED(ServicePassword, Ruqola_ServicePassword, LIBRUQOLACORE_EXPORT)
Q_DECLARE_TYPEINFO(ServicePassword, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ServicePassword &t);
