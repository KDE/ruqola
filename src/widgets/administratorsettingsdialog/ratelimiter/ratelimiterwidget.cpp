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
    , mEnableRateLimiter(new QCheckBox(i18n("Enable Rate Limiter"), this))
    , mLimitByIP(new QCheckBox(i18n("Limit by IP"), this))
    , mLimiteByIpRequestsAllowed(new QSpinBox(this))
    , mLimitByUser(new QCheckBox(i18n("Limit by User"), this))
    , mLimitByConnection(new QCheckBox(i18n("Limit by Connection"), this))
{
    auto apiRateLimiterLabel = createBoldLabel(i18n("API Rate Limiter"));
    apiRateLimiterLabel->setObjectName(QStringLiteral("apiRateLimiterLabel"));
    mMainLayout->addWidget(apiRateLimiterLabel);

    mEnableRateLimiter->setObjectName(QStringLiteral("mEnableRateLimiter"));
    mMainLayout->addWidget(mEnableRateLimiter);
    connectCheckBox(mEnableRateLimiter, QStringLiteral("API_Enable_Rate_Limiter"));

    auto ddpRateLimiterLabel = createBoldLabel(i18n("DDP Rate Limit"));
    ddpRateLimiterLabel->setObjectName(QStringLiteral("ddpRateLimiterLabel"));
    mMainLayout->addWidget(ddpRateLimiterLabel);

    mLimitByIP->setObjectName(QStringLiteral("mLimitByIP"));
    mMainLayout->addWidget(mLimitByIP);
    connectCheckBox(mLimitByIP, QStringLiteral("DDP_Rate_Limit_IP_Enabled"));

    mLimiteByIpRequestsAllowed->setObjectName(QStringLiteral("mLimiteByIpRequestsAllowed"));
    mLimiteByIpRequestsAllowed->setMaximum(9999999);
    addSpinbox(i18n("Limit by IP: requests allowed"), mLimiteByIpRequestsAllowed, QStringLiteral("DDP_Rate_Limit_IP_Requests_Allowed"));

    mLimitByUser->setObjectName(QStringLiteral("mLimitByUser"));
    mMainLayout->addWidget(mLimitByUser);
    connectCheckBox(mLimitByUser, QStringLiteral("DDP_Rate_Limit_User_Enabled"));

    mLimitByConnection->setObjectName(QStringLiteral("mLimitByConnection"));
    mMainLayout->addWidget(mLimitByConnection);
    connectCheckBox(mLimitByConnection, QStringLiteral("DDP_Rate_Limit_Connection_Enabled"));
}

RateLimiterWidget::~RateLimiterWidget() = default;

void RateLimiterWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnableRateLimiter, mapSettings);
    initializeWidget(mLimitByIP, mapSettings);
    initializeWidget(mLimitByUser, mapSettings);
    initializeWidget(mLimitByConnection, mapSettings);
    initializeWidget(mLimiteByIpRequestsAllowed, mapSettings);
}
