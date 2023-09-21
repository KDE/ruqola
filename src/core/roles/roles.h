/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include "role.h"
#include <QJsonObject>
#include <QVector>

class LIBRUQOLACORE_EXPORT Roles
{
public:
    Roles();

    [[nodiscard]] QVector<Role> roles() const;
    void setRoles(const QVector<Role> &roles);

    void parseRole(const QJsonObject &obj);
    [[nodiscard]] Role findRoleByUserId(const QString &userId) const;
    [[nodiscard]] bool isEmpty() const;
    void updateRoles(const QJsonObject &obj);
    [[nodiscard]] int count() const;
    [[nodiscard]] Role at(int index) const;

private:
    QVector<Role> mRoles;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Roles &t);
