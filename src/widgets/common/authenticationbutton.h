/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authenticationinfo.h"
#include "libruqolawidgets_private_export.h"
#include <QPushButton>

class LIBRUQOLAWIDGETS_TESTS_EXPORT AuthenticationButton : public QPushButton
{
    Q_OBJECT
public:
    explicit AuthenticationButton(QWidget *parent = nullptr);
    ~AuthenticationButton() override;
    void setAuthenticationInfo(const AuthenticationInfo &info);

    [[nodiscard]] AuthenticationInfo authenticationInfo() const;

Q_SIGNALS:
    void authentication(AuthenticationManager::AuthMethodType type);

private:
    AuthenticationInfo mAuthenticationInfo;
};
