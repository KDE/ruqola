/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencewidget.h"

#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

VideoConferenceWidget::VideoConferenceWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mBigBlueEnabled(new QCheckBox(i18n("Enabled"), this))
    , mServer(new QLineEdit(this))
    , mSharedSecret(new QLineEdit(this))
    , mAlwaysOpenNewWindow(new QCheckBox(i18n("Always Open in New Window"), this))
{
    auto bigBlueButtonLabel = new QLabel(i18n("BigBlueButton"), this);
    QFont apiRateLimiterLabelFont = bigBlueButtonLabel->font();
    apiRateLimiterLabelFont.setBold(true);
    bigBlueButtonLabel->setFont(apiRateLimiterLabelFont);
    bigBlueButtonLabel->setObjectName(QStringLiteral("bigBlueButtonLabel"));
    mMainLayout->addWidget(bigBlueButtonLabel);

    mBigBlueEnabled->setObjectName(QStringLiteral("mBigBlueEnabled"));
    mMainLayout->addWidget(mBigBlueEnabled);
    connectCheckBox(mBigBlueEnabled, QStringLiteral("bigbluebutton_Enabled"));

    mServer->setObjectName(QStringLiteral("mServer"));
    addLineEdit(i18n("Domain"), mServer, QStringLiteral("bigbluebutton_server"));

    mSharedSecret->setObjectName(QStringLiteral("mSharedSecret"));
    addLineEdit(i18n("Shared Secret"), mSharedSecret, QStringLiteral("bigbluebutton_sharedSecret"));

    mAlwaysOpenNewWindow->setObjectName(QStringLiteral("mAlwaysOpenNewWindow"));
    mMainLayout->addWidget(mAlwaysOpenNewWindow);
    connectCheckBox(mAlwaysOpenNewWindow, QStringLiteral("bigbluebutton_Open_New_Window"));
}

VideoConferenceWidget::~VideoConferenceWidget() = default;

void VideoConferenceWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mBigBlueEnabled, mapSettings);
    initializeWidget(mServer, mapSettings);
    initializeWidget(mSharedSecret, mapSettings);
    initializeWidget(mAlwaysOpenNewWindow, mapSettings);
}
