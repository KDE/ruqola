/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registeruserwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/passwordconfirmwidget.h"

#include <KAuthorized>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>

RegisterUserWidget::RegisterUserWidget(QWidget *parent)
    : QWidget(parent)
    , mRegisterButton(new QPushButton(i18nc("@action:button", "Register"), this))
    , mUserName(new QLineEdit(this))
    , mEmail(new QLineEdit(this))
    , mPasswordConfirmWidget(new PasswordConfirmWidget(this))
    , mReasonTextEdit(new QPlainTextEdit(this))
    , mReasonLabel(new QLabel(i18n("Reason:"), this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mUserName->setObjectName(u"mUserName"_s);
    mainLayout->addRow(i18n("Username:"), mUserName);
    mUserName->setClearButtonEnabled(true);
    connect(mUserName, &QLineEdit::textChanged, this, &RegisterUserWidget::slotUpdateRegisterButton);

    mEmail->setObjectName(u"mEmail"_s);
    mEmail->setClearButtonEnabled(true);
    mainLayout->addRow(i18n("Email:"), mEmail);
    connect(mEmail, &QLineEdit::textChanged, this, &RegisterUserWidget::slotUpdateRegisterButton);

    mPasswordConfirmWidget->setObjectName(u"mPasswordConfirmWidget"_s);
    mainLayout->addRow(mPasswordConfirmWidget);
    connect(mPasswordConfirmWidget, &PasswordConfirmWidget::passwordValidated, this, &RegisterUserWidget::slotUpdateRegisterButton);

    mReasonTextEdit->setObjectName(u"mReasonTextEdit"_s);
    mReasonLabel->setObjectName(u"mReasonLabel"_s);
    mainLayout->addRow(mReasonLabel, mReasonTextEdit);
    // Hide by default
    mReasonLabel->setVisible(false);
    mReasonTextEdit->setVisible(false);
    connect(mReasonTextEdit, &QPlainTextEdit::textChanged, this, &RegisterUserWidget::slotUpdateRegisterButton);

    mRegisterButton->setObjectName(u"mRegisterButton"_s);
    connect(mRegisterButton, &QPushButton::clicked, this, &RegisterUserWidget::slotRegisterNewUser);
    mainLayout->addWidget(mRegisterButton);
    mRegisterButton->setEnabled(false);
}

RegisterUserWidget::~RegisterUserWidget() = default;

void RegisterUserWidget::setPasswordValidChecks(const RuqolaServerConfig::PasswordSettings &passwordSettings)
{
    mPasswordConfirmWidget->setPasswordValidChecks(passwordSettings);
}

void RegisterUserWidget::setManuallyApproveNewUsersRequired(bool manual)
{
    mReasonTextEdit->setVisible(manual);
    mReasonLabel->setVisible(manual);
}

void RegisterUserWidget::slotUpdateRegisterButton()
{
    bool enableRegisterButton =
        !mUserName->text().trimmed().isEmpty() && !mEmail->text().trimmed().isEmpty() && mPasswordConfirmWidget->isNewPasswordConfirmed();
    if (mReasonTextEdit->isVisible()) {
        enableRegisterButton &= !mReasonTextEdit->document()->isEmpty();
    }
    mRegisterButton->setEnabled(enableRegisterButton);
}

void RegisterUserWidget::slotRegisterNewUser()
{
    mRegisterButton->setEnabled(false);
    Q_EMIT registerNewAccount();
}

RocketChatRestApi::RegisterUserJob::RegisterUserInfo RegisterUserWidget::registerUserInfo() const
{
    RocketChatRestApi::RegisterUserJob::RegisterUserInfo info;
    info.email = mEmail->text();
    info.name = mUserName->text();
    info.username = mUserName->text().remove(u' ');
    info.password = mPasswordConfirmWidget->password();
    if (mReasonTextEdit->isVisible()) {
        info.reason = mReasonTextEdit->toPlainText();
    }
    return info;
}

#include "moc_registeruserwidget.cpp"
