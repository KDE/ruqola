/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "ratelimiterwidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>

RateLimiterWidget::RateLimiterWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mEnableRateLimiter(new QCheckBox(this))
{
    auto apiRateLimiterLabel = new QLabel(i18n("API Rate Limiter"), this);
    QFont apiRateLimiterLabelFont = apiRateLimiterLabel->font();
    apiRateLimiterLabelFont.setBold(true);
    apiRateLimiterLabel->setFont(apiRateLimiterLabelFont);
    apiRateLimiterLabel->setObjectName(QStringLiteral("apiRateLimiterLabel"));
    mMainLayout->addWidget(apiRateLimiterLabel);

    mEnableRateLimiter->setObjectName(QStringLiteral("mEnableRateLimiter"));
    mMainLayout->addWidget(mEnableRateLimiter);
    connectCheckBox(mEnableRateLimiter, QStringLiteral("API_Enable_Rate_Limiter"));
}

RateLimiterWidget::~RateLimiterWidget() = default;

void RateLimiterWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnableRateLimiter, mapSettings);
}
