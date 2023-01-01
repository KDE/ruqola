/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include "role.h"
#include <QJsonObject>
#include <QVector>

class LIBRUQOLACORE_TESTS_EXPORT Roles
{
public:
    Roles();

    Q_REQUIRED_RESULT QVector<Role> roles() const;
    void setRoles(const QVector<Role> &roles);

    void parseRole(const QJsonObject &obj);
    Q_REQUIRED_RESULT Role findRoleByUserId(const QString &userId) const;
    Q_REQUIRED_RESULT bool isEmpty() const;
    void updateRoles(const QJsonObject &obj);
    Q_REQUIRED_RESULT int count() const;
    Q_REQUIRED_RESULT Role at(int index) const;

private:
    QVector<Role> mRoles;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Roles &t);
