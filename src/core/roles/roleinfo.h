/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
class LIBRUQOLACORE_EXPORT RoleInfo
{
public:
    RoleInfo();
    ~RoleInfo() = default;

    Q_REQUIRED_RESULT const QString &identifier() const;
    void setIdentifier(const QString &newIdentifier);

    Q_REQUIRED_RESULT const QString &scope() const;
    void setScope(const QString &newScope);

    void parseRoleInfo(const QJsonObject &obj);

    Q_REQUIRED_RESULT const QString &name() const;
    void setName(const QString &newName);

    Q_REQUIRED_RESULT const QString &description() const;
    void setDescription(const QString &newDescription);

    Q_REQUIRED_RESULT bool roleProtected() const;
    void setRoleProtected(bool newRoleProtected);

    Q_REQUIRED_RESULT bool mandatory2fa() const;
    void setMandatory2fa(bool newMandatory2fa);

    Q_REQUIRED_RESULT bool operator==(const RoleInfo &other) const;

private:
    QString mName;
    QString mIdentifier;
    QString mScope;
    QString mDescription;
    bool mRoleProtected = false;
    bool mMandatory2fa = false;
};

Q_DECLARE_METATYPE(RoleInfo)
Q_DECLARE_TYPEINFO(RoleInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const RoleInfo &t);
