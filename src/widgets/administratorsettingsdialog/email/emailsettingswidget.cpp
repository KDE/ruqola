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

    mDebugDirectReply->setObjectName(QStringLiteral("mEnableDirectReply"));
    mMainLayout->addWidget(mDebugDirectReply);
    mDebugDirectReply->setToolTip(i18n("[Beware] Enabling Debug mode would display your 'Plain Text Password' in Admin console."));
    connectCheckBox(mDebugDirectReply, QStringLiteral("Direct_Reply_Debug"));
}

EmailSettingsWidget::~EmailSettingsWidget() = default;

void EmailSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mSmtpProtocol, mapSettings, QStringLiteral("mSmtpProtocol"));
    initializeWidget(mSmtpHost, mapSettings, QString());
    initializeWidget(mSmtpPort, mapSettings, 0);
    initializeWidget(mIgnoreTls, mapSettings, true);
    initializeWidget(mUserName, mapSettings, QString());
    initializeWidget(mFromEmail, mapSettings, QString());
    initializeWidget(mShowMessageEmailNotification, mapSettings, true);
    initializeWidget(mAddSenderReplyTo, mapSettings, false);
    initializeWidget(mEnableDirectReply, mapSettings, false);
}
