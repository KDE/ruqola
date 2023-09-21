/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authenticationmanager.h"
#include "libruqolacore_export.h"
#include <QDebug>
class LIBRUQOLACORE_EXPORT AuthenticationInfo
{
    Q_GADGET
public:
    AuthenticationInfo();
    ~AuthenticationInfo();

    [[nodiscard]] QString name() const;
    void setName(const QString &name);

    [[nodiscard]] QString iconName() const;
    void setIconName(const QString &iconName);

    [[nodiscard]] AuthenticationManager::OauthType oauthType() const;
    void setOauthType(AuthenticationManager::OauthType oauthType);

    [[nodiscard]] bool isValid() const;

private:
    QString mName;
    QString mIconName;
    AuthenticationManager::OauthType mOauthType = AuthenticationManager::OauthType::Unknown;
};
Q_DECLARE_TYPEINFO(AuthenticationInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AuthenticationInfo &t);
