/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class KPasswordLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PasswordConfirmWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PasswordConfirmWidget(QWidget *parent = nullptr);
    ~PasswordConfirmWidget() override;
    [[nodiscard]] bool isNewPasswordConfirmed() const;
    [[nodiscard]] QString password() const;

private:
    KPasswordLineEdit *const mNewPasswordLineEdit;
    KPasswordLineEdit *const mConfirmPasswordLineEdit;
};
