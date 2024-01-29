/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class KPasswordLineEdit;
class QPushButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PasswordLineEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PasswordLineEditWidget(QWidget *parent = nullptr);
    ~PasswordLineEditWidget() override;
    [[nodiscard]] KPasswordLineEdit *passwordLineEdit() const;

    void setAllowPasswordReset(bool allowPassword);

Q_SIGNALS:
    void resetPasswordRequested(const QString &email);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotResetPasswordButton();
    KPasswordLineEdit *const mPasswordLineEdit;
    QPushButton *const mResetPasswordButton;
};
