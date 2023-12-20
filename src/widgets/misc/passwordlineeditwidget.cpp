/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordlineeditwidget.h"
#include "dialogs/resetpassworddialog.h"
#include <KAuthorized>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QHBoxLayout>
#include <QPointer>
#include <QPushButton>

PasswordLineEditWidget::PasswordLineEditWidget(QWidget *parent)
    : QWidget(parent)
    , mPasswordLineEdit(new KPasswordLineEdit(this))
    , mResetPasswordButton(new QPushButton(i18n("Reset Password"), this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mPasswordLineEdit->setObjectName(QStringLiteral("mPasswordLineEdit"));
    mPasswordLineEdit->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));
    mainLayout->addWidget(mPasswordLineEdit);

    mResetPasswordButton->setObjectName(QStringLiteral("mResetPasswordButton"));
    mainLayout->addWidget(mPasswordLineEdit);
    mainLayout->addWidget(mResetPasswordButton);
    connect(mResetPasswordButton, &QPushButton::clicked, this, &PasswordLineEditWidget::slotResetPasswordButton);
}

PasswordLineEditWidget::~PasswordLineEditWidget() = default;

KPasswordLineEdit *PasswordLineEditWidget::passwordLineEdit() const
{
    return mPasswordLineEdit;
}

void PasswordLineEditWidget::setAllowPasswordReset(bool allowPassword)
{
    mResetPasswordButton->setVisible(allowPassword);
}

void PasswordLineEditWidget::slotResetPasswordButton()
{
    QPointer<ResetPasswordDialog> dialog = new ResetPasswordDialog(this);
    if (dialog->exec()) {
        Q_EMIT resetPasswordRequested(dialog->email());
    }
    delete dialog;
}

#include "moc_passwordlineeditwidget.cpp"
