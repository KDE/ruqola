/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "roleinfo.h"

#include <QList>
#include <QObject>

#include "libruqola_private_export.h"
class QJsonObject;

class LIBRUQOLACORE_TESTS_EXPORT RolesManager : public QObject
{
    Q_OBJECT
public:
    explicit RolesManager(QObject *parent = nullptr);
    ~RolesManager() override;
    void parseRoles(const QJsonObject &obj);
    void updateRoles(const QJsonArray &contents);
    [[nodiscard]] const QList<RoleInfo> &roleInfo() const;

Q_SIGNALS:
    void rolesChanged();

private:
    QList<RoleInfo> mRoleInfo;
};
