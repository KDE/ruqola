/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include "permission.h"
#include <QVector>
class RoleInfo;
class LIBRUQOLACORE_EXPORT Permissions
{
public:
    Permissions();

    [[nodiscard]] QVector<Permission> permissions() const;
    void setPermissions(const QVector<Permission> &permissions);

    void parsePermissions(const QJsonObject &obj, const QString &str = {}, const QVector<RoleInfo> &roleInfo = {});
    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] int count() const;
    void clear();

    [[nodiscard]] Permission at(int index) const;

private:
    QVector<Permission> mPermissions;
};
