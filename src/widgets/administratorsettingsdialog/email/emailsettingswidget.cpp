/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "emailsettingswidget.h"
#include <KLocalizedString>
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
    , mIgnoreTls(new QCheckBox(i18n("IgnoreTLS"), this))
    , mUserName(new QLineEdit(this))
    , mFromEmail(new QLineEdit(this))
    , mShowMessageEmailNotification(new QCheckBox(i18n("Show Message in Email Notification"), this))
    , mAddSenderReplyTo(new QCheckBox(i18n("Add Sender to Reply-To"), this))
    , mEnableDirectReply(new QCheckBox(i18n("Enable Direct Reply"), this))
    , mDebugDirectReply(new QCheckBox(i18n("Debug Direct Reply"), this))
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
{
    auto smtpLabel = createBoldLabel(i18n("SMTP"));
    smtpLabel->setObjectName(QStringLiteral("smtpLabel"));
    mMainLayout->addWidget(smtpLabel);

    mSmtpProtocol->setObjectName(QStringLiteral("mSmtpProtocol"));
    const QMap<QString, QString> maps = {
        {QStringLiteral("smtp"), i18n("smtp")},
        {QStringLiteral("smtps"), i18n("smtps")},
    };
    addComboBox(i18n("Protocol"), maps, mSmtpProtocol, QStringLiteral("SMTP_Protocol"));

    mSmtpHost->setObjectName(QStringLiteral("mSmtpHost"));
    addLineEdit(i18n("Host"), mSmtpHost, QStringLiteral("SMTP_Host"));

    mSmtpPort->setObjectName(QStringLiteral("mSmtpPort"));
    mSmtpPort->setMaximum(99999);
    addSpinbox(i18n("Port"), mSmtpPort, QStringLiteral("SMTP_Port"));

    mIgnoreTls->setObjectName(QStringLiteral("mIgnoreTls"));
    mMainLayout->addWidget(mIgnoreTls);
    connectCheckBox(mIgnoreTls, QStringLiteral("SMTP_IgnoreTLS"));

    mUserName->setObjectName(QStringLiteral("mUserName"));
    addLineEdit(i18n("Username"), mUserName, QStringLiteral("SMTP_Username"));

    // Add password

    mFromEmail->setObjectName(QStringLiteral("mFromEmail"));
    addLineEdit(i18n("From Email"), mFromEmail, QStringLiteral("From_Email"));

    auto privacyLabel = createBoldLabel(i18n("Privacy"));
    privacyLabel->setObjectName(QStringLiteral("privacyLabel"));
    mMainLayout->addWidget(privacyLabel);

    mShowMessageEmailNotification->setObjectName(QStringLiteral("mShowMessageEmailNotification"));
    mMainLayout->addWidget(mShowMessageEmailNotification);
    connectCheckBox(mShowMessageEmailNotification, QStringLiteral("Email_notification_show_message"));

    mAddSenderReplyTo->setObjectName(QStringLiteral("mAddSenderReplyTo"));
    mMainLayout->addWidget(mAddSenderReplyTo);
    connectCheckBox(mAddSenderReplyTo, QStringLiteral("Add_Sender_To_ReplyTo"));

    auto directReplyLabel = createBoldLabel(i18n("Direct Reply"));
    directReplyLabel->setObjectName(QStringLiteral("directReplyLabel"));
    mMainLayout->addWidget(directReplyLabel);

    mEnableDirectReply->setObjectName(QStringLiteral("mEnableDirectReply"));
    mMainLayout->addWidget(mEnableDirectReply);
    mEnableDirectReply->setToolTip(
        i18n("[Attention!] If \"Direct Reply\" is enabled, Rocket.Chat will control the configured email mailbox.\n"
             "All unread e-mails are retrieved, marked as read and processed.\n"
             "\"Direct Reply\" should only be activated if the mailbox used is intended exclusively for access by Rocket.Chat\n"
             "and is not read/processed \"in parallel\" by humans."));
    connectCheckBox(mEnableDirectReply, QStringLiteral("Direct_Reply_Enable"));

    mDebugDirectReply->setObjectName(QStringLiteral("mDebugDirectReply"));
    mMainLayout->addWidget(mDebugDirectReply);
    mDebugDirectReply->setToolTip(i18n("[Beware] Enabling Debug mode would display your 'Plain Text Password' in Admin console."));
    connectCheckBox(mDebugDirectReply, QStringLiteral("Direct_Reply_Debug"));

    auto forgotPasswordLabel = createBoldLabel(i18n("Forgot password"));
    forgotPasswordLabel->setObjectName(QStringLiteral("forgotPasswordLabel"));
    mMainLayout->addWidget(forgotPasswordLabel);

    mForgotPasswordSubject->setObjectName(QStringLiteral("mForgotPasswordSubject"));
    addLineEdit(i18n("Subject"), mForgotPasswordSubject, QStringLiteral("Forgot_Password_Email_Subject"));

    mForgotPasswordBody->setObjectName(QStringLiteral("mForgotPasswordBody"));
    addPlainTextEdit(i18n("Body"), mForgotPasswordBody, QStringLiteral("Forgot_Password_Email"));

    auto subjectLabel = createBoldLabel(i18n("Subject"));
    subjectLabel->setObjectName(QStringLiteral("subjectLabel"));
    mMainLayout->addWidget(subjectLabel);

    mDirectMessageEmailSubject->setObjectName(QStringLiteral("mDirectMessageEmailSubject"));
    addPlainTextEdit(i18n("Direct Message Email Subject"), mDirectMessageEmailSubject, QStringLiteral("Offline_DM_Email"));

    mMentionEmailSubject->setObjectName(QStringLiteral("mMentionEmailSubject"));
    addPlainTextEdit(i18n("Mention Email Subject"), mMentionEmailSubject, QStringLiteral("Offline_Mention_Email"));

    mMentionAllEmailSubject->setObjectName(QStringLiteral("mMentionAllEmailSubject"));
    addPlainTextEdit(i18n("Mention All Email Subject"), mMentionAllEmailSubject, QStringLiteral("Offline_Mention_All_Email"));

    auto verificationLabel = createBoldLabel(i18n("Verification"));
    verificationLabel->setObjectName(QStringLiteral("verificationLabel"));
    mMainLayout->addWidget(verificationLabel);

    mVerificationSubject->setObjectName(QStringLiteral("mVerificationSubject"));
    addLineEdit(i18n("Subject"), mVerificationSubject, QStringLiteral("Verification_Email_Subject"));

    mVerificationBody->setObjectName(QStringLiteral("mVerificationBody"));
    addPlainTextEdit(i18n("Body"), mVerificationBody, QStringLiteral("Verification_Email"));

    auto emailAddressChangedLabel = createBoldLabel(i18n("Email Address Changed"));
    emailAddressChangedLabel->setObjectName(QStringLiteral("emailAddressChangedLabel"));
    mMainLayout->addWidget(emailAddressChangedLabel);

    mEmailAddressChangedSubject->setObjectName(QStringLiteral("mEmailAddressChangedSubject"));
    addLineEdit(i18n("Subject"), mEmailAddressChangedSubject, QStringLiteral("Email_Changed_Email_Subject"));

    mEmailAddressChangedBody->setObjectName(QStringLiteral("mEmailAddressChangedBody"));
    addPlainTextEdit(i18n("Body"), mEmailAddressChangedBody, QStringLiteral("Email_Changed_Email"));

    auto invitationLabel = createBoldLabel(i18n("Invitation"));
    invitationLabel->setObjectName(QStringLiteral("invitationLabel"));
    mMainLayout->addWidget(invitationLabel);

    mInvitationSubject->setObjectName(QStringLiteral("mInvitationSubject"));
    addLineEdit(i18n("Subject"), mInvitationSubject, QStringLiteral("Invitation_Subject"));

    mInvitationBody->setObjectName(QStringLiteral("mInvitationBody"));
    addPlainTextEdit(i18n("Body"), mInvitationBody, QStringLiteral("Invitation_Email"));
}

EmailSettingsWidget::~EmailSettingsWidget() = default;

void EmailSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mSmtpProtocol, mapSettings, QStringLiteral("smtp"));
    initializeWidget(mSmtpHost, mapSettings, QString());
    initializeWidget(mSmtpPort, mapSettings, 0);
    initializeWidget(mIgnoreTls, mapSettings, true);
    initializeWidget(mUserName, mapSettings, QString());
    initializeWidget(mFromEmail, mapSettings, QString());
    initializeWidget(mShowMessageEmailNotification, mapSettings, true);
    initializeWidget(mAddSenderReplyTo, mapSettings, false);
    initializeWidget(mEnableDirectReply, mapSettings, false);
    initializeWidget(mDirectMessageEmailSubject, mapSettings, QStringLiteral("[[Site_Name]] You have been direct messaged by [User]"));
    initializeWidget(mMentionEmailSubject, mapSettings, QStringLiteral("[[Site_Name]] You have been mentioned by [User] in #[Room]"));
    initializeWidget(mMentionAllEmailSubject, mapSettings, QStringLiteral("[User] has posted a message in #[Room]"));
    initializeWidget(mVerificationSubject, mapSettings, QStringLiteral("[Site_Name] - Email address verification"));
    initializeWidget(mVerificationBody, mapSettings, QStringLiteral("Click <a href=\"[Verification_Url]\">here</a> to verify your email address."));
    initializeWidget(mForgotPasswordSubject, mapSettings, QStringLiteral("[Site_Name] - Password Recovery"));
    initializeWidget(mForgotPasswordBody, mapSettings, QStringLiteral("Click <a href=\"[Forgot_Password_Url]\">here</a> to reset your password."));

    initializeWidget(mEmailAddressChangedSubject, mapSettings, QStringLiteral("[Site_Name] - Email address has been changed"));
    initializeWidget(mEmailAddressChangedBody, mapSettings, QStringLiteral("Click <a href=\"[Forgot_Password_Url]\">here</a> to reset your password."));

    initializeWidget(mInvitationSubject, mapSettings, QStringLiteral("You have been invited to [Site_Name]"));
    initializeWidget(mInvitationBody, mapSettings, QStringLiteral("<h2>{Welcome_to Site_Name}</h2><p>{Visit_Site_Url_and_try_the_best_open_source_chat_solution_available_today}</p><a class=\"btn\" href=\"[Site_URL]\">{Join_Chat}</a>"));
}
