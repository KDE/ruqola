/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "analyticswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>

AnalyticsWidget::AnalyticsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mMessages(new QCheckBox(i18n("Messages"), this))
    , mRooms(new QCheckBox(i18n("Rooms"), this))
    , mUsers(new QCheckBox(i18n("Users"), this))
{
    auto featuresEnabledLabel = createBoldLabel(i18n("Features Enabled"));
    featuresEnabledLabel->setObjectName(QStringLiteral("featuresEnabledLabel"));
    mMainLayout->addWidget(featuresEnabledLabel);

    mMessages->setObjectName(QStringLiteral("mMessages"));
    mMainLayout->addWidget(mMessages);
    mMessages->setToolTip(i18n("Tracks custom events related to actions a user does on messages."));
    connectCheckBox(mMessages, QStringLiteral("Analytics_features_messages"));

    mRooms->setObjectName(QStringLiteral("mRooms"));
    mMainLayout->addWidget(mRooms);
    mRooms->setToolTip(i18n("Tracks custom events related to actions on a channel or group (create, leave, delete)."));
    connectCheckBox(mRooms, QStringLiteral("Analytics_features_rooms"));

    mUsers->setObjectName(QStringLiteral("mUsers"));
    mMainLayout->addWidget(mUsers);
    mUsers->setToolTip(i18n("Tracks custom events related to actions related to users (password reset times, profile picture change, etc)."));
    connectCheckBox(mUsers, QStringLiteral("Analytics_features_users"));

    auto googleAnalyticsLabel = createBoldLabel(i18n("Google Analytics"));
    googleAnalyticsLabel->setObjectName(QStringLiteral("googleAnalyticsLabel"));
    mMainLayout->addWidget(googleAnalyticsLabel);
}

AnalyticsWidget::~AnalyticsWidget() = default;

void AnalyticsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mMessages, mapSettings, true);
    initializeWidget(mRooms, mapSettings, true);
    initializeWidget(mUsers, mapSettings, true);
}
