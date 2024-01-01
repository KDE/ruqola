/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
class LIBRUQOLACORE_EXPORT RoleInfo
{
public:
    RoleInfo();
    ~RoleInfo() = default;

    [[nodiscard]] const QString &identifier() const;
    void setIdentifier(const QString &newIdentifier);

    [[nodiscard]] const QString &scope() const;
    void setScope(const QString &newScope);

    void parseRoleInfo(const QJsonObject &obj);

    [[nodiscard]] const QString &name() const;
    void setName(const QString &newName);

    [[nodiscard]] const QString &description() const;
    void setDescription(const QString &newDescription);

    [[nodiscard]] bool roleProtected() const;
    void setRoleProtected(bool newRoleProtected);

    [[nodiscard]] bool mandatory2fa() const;
    void setMandatory2fa(bool newMandatory2fa);

    [[nodiscard]] bool operator==(const RoleInfo &other) const;

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
