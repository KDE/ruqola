/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

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

private:
    QVector<Role> mRoles;
};
