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
{
    auto apiRateLimiterLabel = new QLabel(i18n("API Rate Limiter"), this);
    apiRateLimiterLabel->setObjectName(QStringLiteral("apiRateLimiterLabel"));
    mMainLayout->addWidget(apiRateLimiterLabel);
}

RateLimiterWidget::~RateLimiterWidget() = default;

void RateLimiterWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
}
