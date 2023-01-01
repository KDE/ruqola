/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "slackbridgewidget.h"
#include "rocketchataccount.h"

#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPlainTextEdit>

SlackBridgeWidget::SlackBridgeWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnabled(new QCheckBox(i18n("Enabled"), this))
    , mFileUpload(new QCheckBox(i18n("File Upload"), this))
    , mSlackBridgeOutEnabled(new QCheckBox(i18n("SlackBridge Out Enabled"), this))
    , mSlackBridgeOutAll(new QCheckBox(i18n("SlackBridge Out All"), this))
    , mAliasFormat(new QLineEdit(this))
    , mExcludeBots(new QLineEdit(this))
    , mReactions(new QCheckBox(i18n("Reactions"), this))
    , mAPITokens(new QPlainTextEdit(this))
{
    mEnabled->setObjectName(QStringLiteral("mEnabled"));
    mEnabled->setToolTip(i18n("Enable Rocket.Chat to communicate directly with Slack."));
    mMainLayout->addWidget(mEnabled);
    connectCheckBox(mEnabled, QStringLiteral("SlackBridge_Enabled"));

    mAPITokens->setObjectName(QStringLiteral("mAPITokens"));
    addPlainTextEdit(i18n("API Tokens"), mAPITokens, QStringLiteral("SlackBridge_APIToken"));

    mFileUpload->setObjectName(QStringLiteral("mFileUpload"));
    mMainLayout->addWidget(mFileUpload);
    connectCheckBox(mFileUpload, QStringLiteral("SlackBridge_FileUpload_Enabled"));

    mSlackBridgeOutEnabled->setObjectName(QStringLiteral("mSlackBridgeOutEnabled"));
    mSlackBridgeOutEnabled->setToolTip(i18n("Choose whether SlackBridge should also send your messages back to Slack"));
    mMainLayout->addWidget(mSlackBridgeOutEnabled);
    connectCheckBox(mSlackBridgeOutEnabled, QStringLiteral("SlackBridge_Out_Enabled"));

    mSlackBridgeOutAll->setObjectName(QStringLiteral("mSlackBridgeOutAll"));
    mSlackBridgeOutAll->setToolTip(i18n("Send messages from all channels that exist in Slack and the bot has joined"));
    mMainLayout->addWidget(mSlackBridgeOutAll);
    connectCheckBox(mSlackBridgeOutAll, QStringLiteral("SlackBridge_Out_All"));

    mAliasFormat->setObjectName(QStringLiteral("mAliasFormat"));
    mAliasFormat->setToolTip(i18n("Import messages from Slack with an alias; %s is replaced by the username of the user. If empty, no alias will be used."));
    addLineEdit(i18n("Alias Format"), mAliasFormat, QStringLiteral("SlackBridge_AliasFormat"));

    mExcludeBots->setObjectName(QStringLiteral("mExcludeBots"));
    mExcludeBots->setToolTip(
        i18n("Do not propagate messages from bots whose name matches the regular expression above. If left empty, all messages from bots will be propagated."));
    addLineEdit(i18n("Exclude Bots"), mExcludeBots, QStringLiteral("SlackBridge_ExcludeBotnames"));

    mReactions->setObjectName(QStringLiteral("mReactions"));
    mMainLayout->addWidget(mReactions);
    connectCheckBox(mReactions, QStringLiteral("SlackBridge_Reactions_Enabled"));
}

SlackBridgeWidget::~SlackBridgeWidget() = default;

void SlackBridgeWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnabled, mapSettings, false);
    initializeWidget(mFileUpload, mapSettings, true);
    initializeWidget(mSlackBridgeOutEnabled, mapSettings, false);
    initializeWidget(mSlackBridgeOutAll, mapSettings, false);
    initializeWidget(mAliasFormat, mapSettings, {});
    initializeWidget(mExcludeBots, mapSettings, {});
    initializeWidget(mReactions, mapSettings, true);
    initializeWidget(mAPITokens, mapSettings);
}
