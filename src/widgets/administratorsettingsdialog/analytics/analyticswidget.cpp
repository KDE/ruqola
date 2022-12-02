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
{
    auto featuresEnabledLabel = createBoldLabel(i18n("Features Enabled"));
    featuresEnabledLabel->setObjectName(QStringLiteral("featuresEnabledLabel"));
    mMainLayout->addWidget(featuresEnabledLabel);

    mMessages->setObjectName(QStringLiteral("mMessages"));
    mMainLayout->addWidget(mMessages);
    connectCheckBox(mMessages, QStringLiteral("Analytics_features_messages"));
}

AnalyticsWidget::~AnalyticsWidget() = default;

void AnalyticsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mMessages, mapSettings, true);
}
