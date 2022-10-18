/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "slackbridgewidget.h"
#include "rocketchataccount.h"

#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>

SlackBridgeWidget::SlackBridgeWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnabled(new QCheckBox(i18n("Enabled"), this))
    , mFileUpload(new QCheckBox(i18n("File Upload"), this))
    , mSlackBridgeOutEnabled(new QCheckBox(i18n("SlackBridge Out Enabled"), this))
{
    mEnabled->setObjectName(QStringLiteral("mEnabled"));
    mEnabled->setToolTip(i18n("Enable Rocket.Chat to communicate directly with Slack."));
    mMainLayout->addWidget(mEnabled);
    connectCheckBox(mEnabled, QStringLiteral("SlackBridge_Enabled"));

    mFileUpload->setObjectName(QStringLiteral("mFileUpload"));
    mMainLayout->addWidget(mFileUpload);
    connectCheckBox(mFileUpload, QStringLiteral("SlackBridge_FileUpload_Enabled"));

    mSlackBridgeOutEnabled->setObjectName(QStringLiteral("mSlackBridgeOutEnabled"));
    mSlackBridgeOutEnabled->setToolTip(i18n("Choose whether SlackBridge should also send your messages back to Slack"));
    mMainLayout->addWidget(mSlackBridgeOutEnabled);
    connectCheckBox(mSlackBridgeOutEnabled, QStringLiteral("SlackBridge_Out_Enabled"));
}

SlackBridgeWidget::~SlackBridgeWidget() = default;

void SlackBridgeWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnabled, mapSettings, false);
    initializeWidget(mFileUpload, mapSettings, true);
    initializeWidget(mSlackBridgeOutEnabled, mapSettings, false);
}
