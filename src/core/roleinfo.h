/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
