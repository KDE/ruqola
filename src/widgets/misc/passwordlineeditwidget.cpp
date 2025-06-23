/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordlineeditwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/resetpassworddialog.h"
#include <KAuthorized>
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QHBoxLayout>
#include <QPointer>
#include <QPushButton>

PasswordLineEditWidget::PasswordLineEditWidget(QWidget *parent)
    : QWidget(parent)
    , mPasswordLineEdit(new KPasswordLineEdit(this))
    , mResetPasswordButton(new QPushButton(i18nc("@action:button", "Reset Password"), this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mPasswordLineEdit->setObjectName(u"mPasswordLineEdit"_s);
    mPasswordLineEdit->setRevealPasswordMode(KAuthorized::authorize(u"lineedit_reveal_password"_s) ? KPassword::RevealMode::OnlyNew
                                                                                                   : KPassword::RevealMode::Never);
    mainLayout->addWidget(mPasswordLineEdit);

    mResetPasswordButton->setObjectName(u"mResetPasswordButton"_s);
    mainLayout->addWidget(mPasswordLineEdit);
    mainLayout->addWidget(mResetPasswordButton);
    KLineEditEventHandler::catchReturnKey(mPasswordLineEdit->lineEdit());
    connect(mResetPasswordButton, &QPushButton::clicked, this, &PasswordLineEditWidget::slotResetPasswordButton);
    connect(mPasswordLineEdit->lineEdit(), &QLineEdit::returnPressed, this, &PasswordLineEditWidget::tryLogin);
}

PasswordLineEditWidget::~PasswordLineEditWidget() = default;

void PasswordLineEditWidget::setPassword(const QString &password)
{
    mPasswordLineEdit->setPassword(password);
}

QString PasswordLineEditWidget::password() const
{
    return mPasswordLineEdit->password();
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
