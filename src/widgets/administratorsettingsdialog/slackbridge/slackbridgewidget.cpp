/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "slackbridgewidget.h"
using namespace Qt::Literals::StringLiterals;

#include "rocketchataccount.h"

#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPlainTextEdit>

SlackBridgeWidget::SlackBridgeWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnabled(new QCheckBox(i18nc("@option:check", "Enabled"), this))
    , mAPITokens(new QPlainTextEdit(this))
    , mFileUpload(new QCheckBox(i18nc("@option:check", "File Upload"), this))
    , mSlackBridgeOutEnabled(new QCheckBox(i18nc("@option:check", "SlackBridge Out Enabled"), this))
    , mSlackBridgeOutAll(new QCheckBox(i18nc("@option:check", "SlackBridge Out All"), this))
    , mAliasFormat(new QLineEdit(this))
    , mExcludeBots(new QLineEdit(this))
    , mReactions(new QCheckBox(i18nc("@option:check", "Reactions"), this))
{
    mEnabled->setObjectName(u"mEnabled"_s);
    mEnabled->setToolTip(i18nc("@info:tooltip", "Enable Rocket.Chat to communicate directly with Slack."));
    addCheckBox(mEnabled, u"SlackBridge_Enabled"_s);

    mAPITokens->setObjectName(u"mAPITokens"_s);
    addPlainTextEdit(i18n("API Tokens"), mAPITokens, u"SlackBridge_APIToken"_s);

    mFileUpload->setObjectName(u"mFileUpload"_s);
    addCheckBox(mFileUpload, u"SlackBridge_FileUpload_Enabled"_s);

    mSlackBridgeOutEnabled->setObjectName(u"mSlackBridgeOutEnabled"_s);
    mSlackBridgeOutEnabled->setToolTip(i18nc("@info:tooltip", "Choose whether SlackBridge should also send your messages back to Slack"));
    addCheckBox(mSlackBridgeOutEnabled, u"SlackBridge_Out_Enabled"_s);

    mSlackBridgeOutAll->setObjectName(u"mSlackBridgeOutAll"_s);
    mSlackBridgeOutAll->setToolTip(i18nc("@info:tooltip", "Send messages from all channels that exist in Slack and the bot has joined"));
    addCheckBox(mSlackBridgeOutAll, u"SlackBridge_Out_All"_s);

    mAliasFormat->setObjectName(u"mAliasFormat"_s);
    mAliasFormat->setToolTip(
        i18nc("@info:tooltip", "Import messages from Slack with an alias; %s is replaced by the username of the user. If empty, no alias will be used."));
    addLineEdit(i18n("Alias Format"), mAliasFormat, u"SlackBridge_AliasFormat"_s);

    mExcludeBots->setObjectName(u"mExcludeBots"_s);
    mExcludeBots->setToolTip(
        i18n("Do not propagate messages from bots whose name matches the regular expression above. If left empty, all messages from bots will be propagated."));
    addLineEdit(i18n("Exclude Bots"), mExcludeBots, u"SlackBridge_ExcludeBotnames"_s);

    mReactions->setObjectName(u"mReactions"_s);
    addCheckBox(mReactions, u"SlackBridge_Reactions_Enabled"_s);
}

SlackBridgeWidget::~SlackBridgeWidget() = default;

void SlackBridgeWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
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

#include "moc_slackbridgewidget.cpp"
