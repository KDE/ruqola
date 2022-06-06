/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordlineeditwidget.h"
#include <KAuthorized>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QHBoxLayout>
#include <QInputDialog>
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

void PasswordLineEditWidget::slotResetPasswordButton()
{
    bool ok = false;
    const QString email = QInputDialog::getText(this, i18n("Reset Password"), i18n("Email:"), QLineEdit::Normal, {}, &ok);
    if (ok && !email.trimmed().isEmpty()) {
        // Validate email ?
        Q_EMIT resetPasswordRequested(email);
    }
}
