/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "ruqolaserverconfig.h"
#include <QWidget>
class KPasswordLineEdit;
class PasswordValidateWidget;
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PasswordConfirmWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PasswordConfirmWidget(QWidget *parent = nullptr);
    ~PasswordConfirmWidget() override;
    [[nodiscard]] bool isNewPasswordConfirmed() const;
    [[nodiscard]] QString password() const;

    void setPasswordValidChecks(const RuqolaServerConfig::PasswordSettings &passwordSettings);

Q_SIGNALS:
    void passwordValidated(bool state);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotVerifyPassword();
    KPasswordLineEdit *const mNewPasswordLineEdit;
    KPasswordLineEdit *const mConfirmPasswordLineEdit;
    PasswordValidateWidget *const mPasswordValidateWidget;
    QLabel *const mInvalidPassword;
};
