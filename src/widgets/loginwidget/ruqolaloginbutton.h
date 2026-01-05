/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QPushButton>

class LIBRUQOLAWIDGETS_TESTS_EXPORT RuqolaLoginButton : public QPushButton
{
    Q_OBJECT
public:
    explicit RuqolaLoginButton(QWidget *parent = nullptr);
    ~RuqolaLoginButton() override;

    [[nodiscard]] bool loginInProgress() const;
    void setLoginInProgress(bool newLoginInProgress);

private:
    bool mLoginInProgress = false;
};
