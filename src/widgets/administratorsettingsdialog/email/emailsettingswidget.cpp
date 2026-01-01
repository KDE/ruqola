/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "emailsettingswidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QSpinBox>

EmailSettingsWidget::EmailSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mSmtpProtocol(new QComboBox(this))
    , mSmtpHost(new QLineEdit(this))
    , mSmtpPort(new QSpinBox(this))
    , mSmtpIgnoreTls(new QCheckBox(i18nc("@option:check", "IgnoreTLS"), this))
    , mSmtpUserName(new QLineEdit(this))
    , mSmtpPassword(new KPasswordLineEdit(this))
    , mSmtpFromEmail(new QLineEdit(this))
    , mShowMessageEmailNotification(new QCheckBox(i18nc("@option:check", "Show Message in Email Notification"), this))
    , mAddSenderReplyTo(new QCheckBox(i18nc("@option:check", "Add Sender to Reply-To"), this))
    , mEnableDirectReply(new QCheckBox(i18nc("@option:check", "Enable Direct Reply"), this))
    , mDebugDirectReply(new QCheckBox(i18nc("@option:check", "Debug Direct Reply"), this))
    , mDirectMessageEmailSubject(new QPlainTextEdit(this))
    , mMentionEmailSubject(new QPlainTextEdit(this))
    , mMentionAllEmailSubject(new QPlainTextEdit(this))
    , mVerificationSubject(new QLineEdit(this))
    , mVerificationBody(new QPlainTextEdit(this))
    , mForgotPasswordSubject(new QLineEdit(this))
    , mForgotPasswordBody(new QPlainTextEdit(this))
    , mEmailAddressChangedSubject(new QLineEdit(this))
    , mEmailAddressChangedBody(new QPlainTextEdit(this))
    , mInvitationSubject(new QLineEdit(this))
    , mInvitationBody(new QPlainTextEdit(this))
    , mPasswordChangedSubject(new QLineEdit(this))
    , mPasswordChangedBody(new QPlainTextEdit(this))
{
    auto smtpLabel = createBoldLabel(i18n("SMTP"));
    smtpLabel->setObjectName(u"smtpLabel"_s);
    mMainLayout->addWidget(smtpLabel);

    mSmtpProtocol->setObjectName(u"mSmtpProtocol"_s);
    const QMap<QString, QString> maps = {
        {u"smtp"_s, i18n("smtp")},
        {u"smtps"_s, i18n("smtps")},
    };
    addComboBox(i18n("Protocol"), maps, mSmtpProtocol, u"SMTP_Protocol"_s);

    mSmtpHost->setObjectName(u"mSmtpHost"_s);
    addLineEdit(i18n("Host"), mSmtpHost, u"SMTP_Host"_s);

    mSmtpPort->setObjectName(u"mSmtpPort"_s);
    mSmtpPort->setMaximum(99999);
    addSpinbox(i18n("Port"), mSmtpPort, u"SMTP_Port"_s);

    mSmtpIgnoreTls->setObjectName(u"mIgnoreTls"_s);
    addCheckBox(mSmtpIgnoreTls, u"SMTP_IgnoreTLS"_s);

    mSmtpUserName->setObjectName(u"mUserName"_s);
    addLineEdit(i18n("Username"), mSmtpUserName, u"SMTP_Username"_s);

    mSmtpPassword->setObjectName(u"mSmtpPassword"_s);
    addPasswordEdit(i18n("Password"), mSmtpPassword, u"SMTP_Password"_s);

    mSmtpFromEmail->setObjectName(u"mFromEmail"_s);
    addLineEdit(i18n("From Email"), mSmtpFromEmail, u"From_Email"_s);

    auto privacyLabel = createBoldLabel(i18n("Privacy"));
    privacyLabel->setObjectName(u"privacyLabel"_s);
    mMainLayout->addWidget(privacyLabel);

    mShowMessageEmailNotification->setObjectName(u"mShowMessageEmailNotification"_s);
    addCheckBox(mShowMessageEmailNotification, u"Email_notification_show_message"_s);

    mAddSenderReplyTo->setObjectName(u"mAddSenderReplyTo"_s);
    addCheckBox(mAddSenderReplyTo, u"Add_Sender_To_ReplyTo"_s);

    auto directReplyLabel = createBoldLabel(i18n("Direct Reply"));
    directReplyLabel->setObjectName(u"directReplyLabel"_s);
    mMainLayout->addWidget(directReplyLabel);

    mEnableDirectReply->setObjectName(u"mEnableDirectReply"_s);
    mEnableDirectReply->setToolTip(
        i18n("[Attention!] If \"Direct Reply\" is enabled, Rocket.Chat will control the configured email mailbox.\n"
             "All unread e-mails are retrieved, marked as read and processed.\n"
             "\"Direct Reply\" should only be activated if the mailbox used is intended exclusively for access by Rocket.Chat\n"
             "and is not read/processed \"in parallel\" by humans."));
    addCheckBox(mEnableDirectReply, u"Direct_Reply_Enable"_s);

    mDebugDirectReply->setObjectName(u"mDebugDirectReply"_s);
    mDebugDirectReply->setToolTip(i18nc("@info:tooltip", "[Beware] Enabling Debug mode would display your 'Plain Text Password' in Admin console."));
    addCheckBox(mDebugDirectReply, u"Direct_Reply_Debug"_s);

    auto forgotPasswordLabel = createBoldLabel(i18n("Forgot password"));
    forgotPasswordLabel->setObjectName(u"forgotPasswordLabel"_s);
    mMainLayout->addWidget(forgotPasswordLabel);

    mForgotPasswordSubject->setObjectName(u"mForgotPasswordSubject"_s);
    addLineEdit(i18n("Subject"), mForgotPasswordSubject, u"Forgot_Password_Email_Subject"_s);

    mForgotPasswordBody->setObjectName(u"mForgotPasswordBody"_s);
    addPlainTextEdit(i18n("Body"), mForgotPasswordBody, u"Forgot_Password_Email"_s);

    auto subjectLabel = createBoldLabel(i18n("Subject"));
    subjectLabel->setObjectName(u"subjectLabel"_s);
    mMainLayout->addWidget(subjectLabel);

    mDirectMessageEmailSubject->setObjectName(u"mDirectMessageEmailSubject"_s);
    addPlainTextEdit(i18n("Direct Message Email Subject"), mDirectMessageEmailSubject, u"Offline_DM_Email"_s);

    mMentionEmailSubject->setObjectName(u"mMentionEmailSubject"_s);
    addPlainTextEdit(i18n("Mention Email Subject"), mMentionEmailSubject, u"Offline_Mention_Email"_s);

    mMentionAllEmailSubject->setObjectName(u"mMentionAllEmailSubject"_s);
    addPlainTextEdit(i18n("Mention All Email Subject"), mMentionAllEmailSubject, u"Offline_Mention_All_Email"_s);

    auto verificationLabel = createBoldLabel(i18n("Verification"));
    verificationLabel->setObjectName(u"verificationLabel"_s);
    mMainLayout->addWidget(verificationLabel);

    mVerificationSubject->setObjectName(u"mVerificationSubject"_s);
    addLineEdit(i18n("Subject"), mVerificationSubject, u"Verification_Email_Subject"_s);

    mVerificationBody->setObjectName(u"mVerificationBody"_s);
    addPlainTextEdit(i18n("Body"), mVerificationBody, u"Verification_Email"_s);

    auto emailAddressChangedLabel = createBoldLabel(i18n("Email Address Changed"));
    emailAddressChangedLabel->setObjectName(u"emailAddressChangedLabel"_s);
    mMainLayout->addWidget(emailAddressChangedLabel);

    mEmailAddressChangedSubject->setObjectName(u"mEmailAddressChangedSubject"_s);
    addLineEdit(i18n("Subject"), mEmailAddressChangedSubject, u"Email_Changed_Email_Subject"_s);

    mEmailAddressChangedBody->setObjectName(u"mEmailAddressChangedBody"_s);
    addPlainTextEdit(i18n("Body"), mEmailAddressChangedBody, u"Email_Changed_Email"_s);

    auto invitationLabel = createBoldLabel(i18n("Invitation"));
    invitationLabel->setObjectName(u"invitationLabel"_s);
    mMainLayout->addWidget(invitationLabel);

    mInvitationSubject->setObjectName(u"mInvitationSubject"_s);
    addLineEdit(i18n("Subject"), mInvitationSubject, u"Invitation_Subject"_s);

    mInvitationBody->setObjectName(u"mInvitationBody"_s);
    addPlainTextEdit(i18n("Body"), mInvitationBody, u"Invitation_Email"_s);

    auto passwordChangedLabel = createBoldLabel(i18n("Password Changed"));
    passwordChangedLabel->setObjectName(u"passwordChangedLabel"_s);
    mMainLayout->addWidget(passwordChangedLabel);

    mPasswordChangedSubject->setObjectName(u"mPasswordChangedSubject"_s);
    addLineEdit(i18n("Subject"), mPasswordChangedSubject, u"Password_Changed_Email_Subject"_s);

    mPasswordChangedBody->setObjectName(u"mPasswordChangedBody"_s);
    addPlainTextEdit(i18n("Body"), mPasswordChangedBody, u"Password_Changed_Email"_s);
}

EmailSettingsWidget::~EmailSettingsWidget() = default;

void EmailSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mSmtpProtocol, mapSettings, u"smtp"_s);
    initializeWidget(mSmtpHost, mapSettings, QString());
    initializeWidget(mSmtpPort, mapSettings, 0);
    initializeWidget(mSmtpIgnoreTls, mapSettings, true);
    initializeWidget(mSmtpUserName, mapSettings, QString());
    initializeWidget(mSmtpFromEmail, mapSettings, QString());
    initializeWidget(mSmtpPassword, mapSettings);
    initializeWidget(mShowMessageEmailNotification, mapSettings, true);
    initializeWidget(mAddSenderReplyTo, mapSettings, false);
    initializeWidget(mEnableDirectReply, mapSettings, false);
    initializeWidget(mDirectMessageEmailSubject, mapSettings, u"[[Site_Name]] You have been direct messaged by [User]"_s);
    initializeWidget(mMentionEmailSubject, mapSettings, u"[[Site_Name]] You have been mentioned by [User] in #[Room]"_s);
    initializeWidget(mMentionAllEmailSubject, mapSettings, u"[User] has posted a message in #[Room]"_s);
    initializeWidget(mVerificationSubject, mapSettings, u"[Site_Name] - Email address verification"_s);
    initializeWidget(mVerificationBody, mapSettings, u"Click <a href=\"[Verification_Url]\">here</a> to verify your email address."_s);
    initializeWidget(mForgotPasswordSubject, mapSettings, u"[Site_Name] - Password Recovery"_s);
    initializeWidget(mForgotPasswordBody, mapSettings, u"Click <a href=\"[Forgot_Password_Url]\">here</a> to reset your password."_s);

    initializeWidget(mEmailAddressChangedSubject, mapSettings, u"[Site_Name] - Email address has been changed"_s);
    initializeWidget(mEmailAddressChangedBody, mapSettings, u"Click <a href=\"[Forgot_Password_Url]\">here</a> to reset your password."_s);

    initializeWidget(mInvitationSubject, mapSettings, u"You have been invited to [Site_Name]"_s);
    initializeWidget(mInvitationBody,
                     mapSettings,
                     QStringLiteral("<h2>{Welcome_to Site_Name}</h2><p>{Visit_Site_Url_and_try_the_best_open_source_chat_solution_available_today}</p><a "
                                    "class=\"btn\" href=\"[Site_URL]\">{Join_Chat}</a>"));

    initializeWidget(mPasswordChangedSubject, mapSettings, u"{Password_Changed_Email_Subject}"_s);
    initializeWidget(mPasswordChangedBody,
                     mapSettings,
                     QStringLiteral("<h2>{Hi},</h2><p>{Your_password_was_changed_by_an_admin}</p><p>{Your_temporary_password_is_password}</p><a class=\"btn\" "
                                    "target=\"_blank\" href=\"[Site_URL]\">{Login}</a>"));
}

#include "moc_emailsettingswidget.cpp"
