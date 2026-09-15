/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registeruserwidget.h"

#include "misc/passwordconfirmwidget.h"

#include <KLocalizedString>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTextDocument>

using namespace Qt::Literals::StringLiterals;
RegisterUserWidget::RegisterUserWidget(QWidget *parent)
    : QWidget(parent)
    , mRegisterButton(new QPushButton(i18nc("@action:button", "Register"), this))
    , mUserName(new QLineEdit(this))
    , mEmail(new QLineEdit(this))
    , mPasswordConfirmWidget(new PasswordConfirmWidget(this))
    , mReasonTextEdit(new QPlainTextEdit(this))
    , mMainLayout(new QFormLayout(this))
{
    auto reasonLabel = new QLabel(i18n("Reason:"), this);
    mMainLayout->setObjectName(u"mainLayout"_s);
    mMainLayout->setContentsMargins({});

    mUserName->setObjectName(u"mUserName"_s);
    mMainLayout->addRow(i18n("Username:"), mUserName);
    mUserName->setClearButtonEnabled(true);
    connect(mUserName, &QLineEdit::textChanged, this, &RegisterUserWidget::slotUpdateRegisterButton);

    mEmail->setObjectName(u"mEmail"_s);
    mEmail->setClearButtonEnabled(true);
    mMainLayout->addRow(i18n("Email:"), mEmail);
    connect(mEmail, &QLineEdit::textChanged, this, &RegisterUserWidget::slotUpdateRegisterButton);

    mPasswordConfirmWidget->setObjectName(u"mPasswordConfirmWidget"_s);
    mMainLayout->addRow(mPasswordConfirmWidget);
    connect(mPasswordConfirmWidget, &PasswordConfirmWidget::passwordValidated, this, &RegisterUserWidget::slotUpdateRegisterButton);

    mReasonTextEdit->setObjectName(u"mReasonTextEdit"_s);
    reasonLabel->setObjectName(u"mReasonLabel"_s);
    mMainLayout->addRow(reasonLabel, mReasonTextEdit);
    // Hide by default
    mMainLayout->setRowVisible(mReasonTextEdit, mManuallyApproveNewUsersRequired);
    connect(mReasonTextEdit, &QPlainTextEdit::textChanged, this, &RegisterUserWidget::slotUpdateRegisterButton);

    mRegisterButton->setObjectName(u"mRegisterButton"_s);
    connect(mRegisterButton, &QPushButton::clicked, this, &RegisterUserWidget::slotRegisterNewUser);
    mMainLayout->addWidget(mRegisterButton);
    mRegisterButton->setEnabled(false);
}

RegisterUserWidget::~RegisterUserWidget() = default;

void RegisterUserWidget::setPasswordValidChecks(const RuqolaServerConfig::PasswordSettings &passwordSettings)
{
    mPasswordConfirmWidget->setPasswordValidChecks(passwordSettings);
}

void RegisterUserWidget::setManuallyApproveNewUsersRequired(bool manual)
{
    mManuallyApproveNewUsersRequired = manual;
    mMainLayout->setRowVisible(mReasonTextEdit, mManuallyApproveNewUsersRequired);
}

void RegisterUserWidget::slotUpdateRegisterButton()
{
    bool enableRegisterButton =
        !mUserName->text().trimmed().isEmpty() && !mEmail->text().trimmed().isEmpty() && mPasswordConfirmWidget->isNewPasswordConfirmed();
    if (mManuallyApproveNewUsersRequired) {
        enableRegisterButton &= !mReasonTextEdit->toPlainText().trimmed().isEmpty();
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
    info.email = mEmail->text().trimmed();

    QString name = mUserName->text().trimmed();
    info.username = name;
    info.username.remove(u' ');
    info.name = std::move(name);

    info.password = mPasswordConfirmWidget->password();
    if (mManuallyApproveNewUsersRequired) {
        info.reason = mReasonTextEdit->toPlainText().trimmed();
    }
    return info;
}

#include "moc_registeruserwidget.cpp"
