/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolacore_export.h"
#include <QMetaType>
#include <QString>
class QDebug;
class QJsonObject;
class LIBRUQOLACORE_EXPORT BannedUser
{
public:
    BannedUser();
    ~BannedUser();

    [[nodiscard]] QString userName() const;
    void setUserName(const QString &newUserName);

    [[nodiscard]] QString name() const;
    void setName(const QString &newName);

    void parseBannedUser(const QJsonObject &obj);

    [[nodiscard]] QString identifier() const;
    void setIdentifier(const QString &newIndentifier);

    [[nodiscard]] bool operator==(const BannedUser &other) const;

private:
    QString mUserName;
    QString mName;
    QString mIdentifier;
};
QT_DECL_METATYPE_EXTERN_TAGGED(BannedUser, Ruqola_BannedUser, LIBRUQOLACORE_EXPORT)
Q_DECLARE_TYPEINFO(BannedUser, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const BannedUser &t);
