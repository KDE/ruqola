/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include "permission.h"
#include "roles/roleinfo.h"
#include <QJsonObject>
#include <QVector>

class LIBRUQOLACORE_EXPORT Permissions
{
public:
    Permissions();

    Q_REQUIRED_RESULT QVector<Permission> permissions() const;
    void setPermissions(const QVector<Permission> &permissions);

    void parsePermissions(const QJsonObject &obj, const QString &str = {}, const QVector<RoleInfo> &roleInfo = {});
    Q_REQUIRED_RESULT bool isEmpty() const;

    Q_REQUIRED_RESULT int count() const;
    void clear();

    Q_REQUIRED_RESULT Permission at(int index) const;

private:
    QVector<Permission> mPermissions;
};
