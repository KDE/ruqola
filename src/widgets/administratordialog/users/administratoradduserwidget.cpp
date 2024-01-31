/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratoradduserwidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "misc/rolescombobox.h"
#include "rocketchataccount.h"
#include "user.h"
#include <KAuthorized>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPlainTextEdit>

AdministratorAddUserWidget::AdministratorAddUserWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mName(new QLineEdit(this))
    , mUserName(new QLineEdit(this))
    , mStatusText(new QLineEdit(this))
    , mEmail(new QLineEdit(this))
    , mNickName(new QLineEdit(this))
    , mPasswordLineEdit(new KPasswordLineEdit(this))
    , mJoinDefaultChannels(new QCheckBox(i18n("Join Default Channels"), this))
    , mSendWelcomeEmails(new QCheckBox(i18n("Send Welcome Email"), this))
    , mRequirePassword(new QCheckBox(i18n("Require Password Change"), this))
    , mSetRandowPassword(new QCheckBox(i18n("Set random password and send by email"), this))
    , mEmailVerified(new QCheckBox(i18n("Verified"), this))
    , mRolesComboBox(new RolesComboBox(this))
    , mBioPlainTextEdit(new QPlainTextEdit(this))
    , mRocketChatAccount(account)
{
    auto formLayout = new QFormLayout(this);
    formLayout->setObjectName(QStringLiteral("formLayout"));
    mName->setObjectName(QStringLiteral("mName"));
    mUserName->setObjectName(QStringLiteral("mUserName"));
    mEmail->setObjectName(QStringLiteral("mEmail"));
    mStatusText->setObjectName(QStringLiteral("mStatusText"));
    mBioPlainTextEdit->setObjectName(QStringLiteral("mBioPlainTextEdit"));
    mNickName->setObjectName(QStringLiteral("mNickName"));
    new LineEditCatchReturnKey(mName, this);
    new LineEditCatchReturnKey(mUserName, this);
    new LineEditCatchReturnKey(mEmail, this);
    new LineEditCatchReturnKey(mStatusText, this);
    new LineEditCatchReturnKey(mNickName, this);

    mJoinDefaultChannels->setObjectName(QStringLiteral("mJoinDefaultChannels"));
    mSendWelcomeEmails->setObjectName(QStringLiteral("mSendWelcomeEmails"));
    mPasswordLineEdit->setObjectName(QStringLiteral("mPasswordLineEdit"));
    mPasswordLineEdit->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));
    mRolesComboBox->setObjectName(QStringLiteral("mRolesComboBox"));
    mRequirePassword->setObjectName(QStringLiteral("mRequirePassword"));
    mSetRandowPassword->setObjectName(QStringLiteral("mSetRandowPassword"));
    mEmailVerified->setObjectName(QStringLiteral("mEmailVerified"));
    formLayout->addRow(i18n("Name:"), mName);
    formLayout->addRow(i18n("Username:"), mUserName);
    formLayout->addRow(i18n("Status Message:"), mStatusText);
    formLayout->addRow(i18n("Email:"), mEmail);
    formLayout->addWidget(mEmailVerified);
    formLayout->addRow(i18n("Password:"), mPasswordLineEdit);
    formLayout->addRow(i18n("Bio:"), mBioPlainTextEdit);
    formLayout->addRow(i18n("Nickname:"), mNickName);
    formLayout->addWidget(mJoinDefaultChannels);
    formLayout->addWidget(mSendWelcomeEmails);
    formLayout->addWidget(mRequirePassword);
    formLayout->addWidget(mSetRandowPassword);
    formLayout->addRow(i18n("Roles:"), mRolesComboBox);
    connect(mName, &QLineEdit::textChanged, this, &AdministratorAddUserWidget::slotUpdateOkButton);
    connect(mUserName, &QLineEdit::textChanged, this, &AdministratorAddUserWidget::slotUpdateOkButton);
    connect(mEmail, &QLineEdit::textChanged, this, &AdministratorAddUserWidget::slotUpdateOkButton);
    connect(mPasswordLineEdit, &KPasswordLineEdit::passwordChanged, this, &AdministratorAddUserWidget::slotUpdateOkButton);
    connect(mSetRandowPassword, &QCheckBox::clicked, this, &AdministratorAddUserWidget::slotChangeSetRandomPassword);
}

AdministratorAddUserWidget::~AdministratorAddUserWidget() = default;

void AdministratorAddUserWidget::slotChangeSetRandomPassword(bool checked)
{
    mRequirePassword->setChecked(checked);
    mRequirePassword->setEnabled(!checked);
    mPasswordLineEdit->setEnabled(!checked);
    slotUpdateOkButton();
}

void AdministratorAddUserWidget::slotUpdateOkButton()
{
    bool enableOkButton = !mName->text().trimmed().isEmpty() && !mUserName->text().trimmed().isEmpty() && !mEmail->text().trimmed().isEmpty();
    if (mUserType == UserType::Create) {
        if (mPasswordLineEdit->isEnabled()) {
            enableOkButton &= !mPasswordLineEdit->password().isEmpty();
        }
    }
    Q_EMIT updateButtonOk(enableOkButton);
}

RocketChatRestApi::UpdateUserInfo AdministratorAddUserWidget::updateInfo() const
{
    RocketChatRestApi::UpdateUserInfo info;
    info.mName = mName->text().trimmed();
    info.mEmail = mEmail->text().trimmed();
    info.mUserName = mUserName->text();
    info.mSendWelcomeEmail = mSendWelcomeEmails->isChecked();
    info.mJoinDefaultChannels = mJoinDefaultChannels->isChecked();
    info.mPassword = mPasswordLineEdit->password();
    info.mStatusText = mStatusText->text().trimmed();

    info.mRequirePasswordChange = mRequirePassword->isChecked();
    info.mSetRandomPassword = mSetRandowPassword->isChecked();
    info.mRoles = mRolesComboBox->roles();
    info.mBio = mBioPlainTextEdit->toPlainText();
    info.mVerified = mEmailVerified->isChecked();
    info.mNickName = mNickName->text().trimmed();
    return info;
}

void AdministratorAddUserWidget::setRoleInfo(const QVector<RoleInfo> &info)
{
    mRolesComboBox->setRolesInfo(info);
}

RocketChatRestApi::CreateUpdateUserInfo AdministratorAddUserWidget::createInfo() const
{
    RocketChatRestApi::CreateUpdateUserInfo info;
    info.mName = mName->text().trimmed();
    info.mEmail = mEmail->text().trimmed();
    info.mUserName = mUserName->text();
    info.mStatusText = mStatusText->text().trimmed();
    info.mSendWelcomeEmail = mSendWelcomeEmails->isChecked();
    info.mJoinDefaultChannels = mJoinDefaultChannels->isChecked();
    if (mRocketChatAccount && mRocketChatAccount->hasPermission(QStringLiteral("edit-other-user-password"))) {
        info.mPassword = mPasswordLineEdit->password();
    }
    info.mRequirePasswordChange = mRequirePassword->isChecked();
    info.mSetRandomPassword = mSetRandowPassword->isChecked();
    info.mRoles = mRolesComboBox->roles();
    info.mBio = mBioPlainTextEdit->toPlainText();
    info.mVerified = mEmailVerified->isChecked();
    info.mNickName = mNickName->text().trimmed();
    return info;
}

void AdministratorAddUserWidget::setUser(const User &user)
{
    mUserType = UserType::Edit;
    mName->setText(user.name());
    mUserName->setText(user.userName());
    mEmail->setText(user.userEmailsInfo().email);
    mRolesComboBox->setRoles(user.roles());
    mStatusText->setText(user.statusText());
    mSetRandowPassword->setChecked(user.requirePasswordChange());
    mBioPlainTextEdit->setPlainText(user.bio());
    mNickName->setText(user.nickName());

    mJoinDefaultChannels->setVisible(false);
    mSendWelcomeEmails->setVisible(false);
    // mSetRandowPassword
    // mRequirePasswordChange
    // TODO add mSendWelcomeEmail and mJoinDefaultChannels
    // mJoinDefaultChannels->setChecked(user.jo)
    if (mRocketChatAccount) {
        mPasswordLineEdit->setEnabled(mRocketChatAccount->hasPermission(QStringLiteral("edit-other-user-password")));
    }
}

#include "moc_administratoradduserwidget.cpp"
