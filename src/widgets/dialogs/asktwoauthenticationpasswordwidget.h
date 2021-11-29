/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class TwoAuthenticationPasswordWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AskTwoAuthenticationPasswordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AskTwoAuthenticationPasswordWidget(QWidget *parent = nullptr);
    ~AskTwoAuthenticationPasswordWidget() override;
    Q_REQUIRED_RESULT QString code() const;
Q_SIGNALS:
    void updateButtonOk(bool state);

private:
    TwoAuthenticationPasswordWidget *const mTwoFactorAuthenticationPasswordLineEdit;
};

