/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

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
    , mLimiteByIpIntervalTime(new QSpinBox(this))
    , mLimitByUser(new QCheckBox(i18n("Limit by User"), this))
    , mLimiteByUserRequestsAllowed(new QSpinBox(this))
    , mLimiteByUserIntervalTime(new QSpinBox(this))
    , mLimitByConnection(new QCheckBox(i18n("Limit by Connection"), this))
    , mLimiteByConnectionRequestsAllowed(new QSpinBox(this))
    , mLimiteByConnectionIntervalTime(new QSpinBox(this))

    , mLimitByUserPerMethod(new QCheckBox(i18n("Limit by User per Method"), this))
    , mLimiteByUserRequestsAllowedPerMethod(new QSpinBox(this))
    , mLimiteByUserIntervalTimePerMethod(new QSpinBox(this))
    , mLimitByConnectionPerMethod(new QCheckBox(i18n("Limit by Connection per Method"), this))
    , mLimiteByConnectionRequestsAllowedPerMethod(new QSpinBox(this))
    , mLimiteByConnectionIntervalTimePerMethod(new QSpinBox(this))
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
    addSpinbox(i18n("Limit by IP: interval time"), mLimiteByIpRequestsAllowed, QStringLiteral("DDP_Rate_Limit_IP_Requests_Allowed"));

    mLimiteByIpIntervalTime->setObjectName(QStringLiteral("mLimiteByIpIntervalTime"));
    mLimiteByIpIntervalTime->setMaximum(9999999);
    addSpinbox(i18n("Limit by IP: requests allowed"), mLimiteByIpIntervalTime, QStringLiteral("DDP_Rate_Limit_IP_Interval_Time"));

    mLimitByUser->setObjectName(QStringLiteral("mLimitByUser"));
    mMainLayout->addWidget(mLimitByUser);
    connectCheckBox(mLimitByUser, QStringLiteral("DDP_Rate_Limit_User_Enabled"));

    mLimiteByUserRequestsAllowed->setObjectName(QStringLiteral("mLimiteByUserRequestsAllowed"));
    mLimiteByUserRequestsAllowed->setMaximum(9999999);
    addSpinbox(i18n("Limit by User: requests allowed"), mLimiteByUserRequestsAllowed, QStringLiteral("DDP_Rate_Limit_User_Requests_Allowed"));

    mLimiteByUserIntervalTime->setObjectName(QStringLiteral("mLimiteByUserIntervalTime"));
    mLimiteByUserIntervalTime->setMaximum(9999999);
    addSpinbox(i18n("Limit by User: interval time"), mLimiteByUserIntervalTime, QStringLiteral("DDP_Rate_Limit_User_Interval_Time"));

    mLimitByConnection->setObjectName(QStringLiteral("mLimitByConnection"));
    mMainLayout->addWidget(mLimitByConnection);
    connectCheckBox(mLimitByConnection, QStringLiteral("DDP_Rate_Limit_Connection_Enabled"));

    mLimiteByConnectionRequestsAllowed->setObjectName(QStringLiteral("mLimiteByConnectionRequestsAllowed"));
    mLimiteByConnectionRequestsAllowed->setMaximum(9999999);
    addSpinbox(i18n("Limit by User: requests allowed"), mLimiteByConnectionRequestsAllowed, QStringLiteral("DDP_Rate_Limit_Connection_Requests_Allowed"));

    mLimiteByConnectionIntervalTime->setObjectName(QStringLiteral("mLimiteByConnectionIntervalTime"));
    mLimiteByConnectionIntervalTime->setMaximum(9999999);
    addSpinbox(i18n("Limit by User: interval time"), mLimiteByConnectionIntervalTime, QStringLiteral("DDP_Rate_Limit_Connection_Interval_Time"));

    mLimitByUserPerMethod->setObjectName(QStringLiteral("mLimitByUserPerMethod"));
    mMainLayout->addWidget(mLimitByUserPerMethod);
    connectCheckBox(mLimitByUserPerMethod, QStringLiteral("DDP_Rate_Limit_User_By_Method_Enabled"));

    mLimiteByUserRequestsAllowedPerMethod->setObjectName(QStringLiteral("mLimiteByUserRequestsAllowedPerMethod"));
    mLimiteByUserRequestsAllowedPerMethod->setMaximum(9999999);
    addSpinbox(i18n("Limit by User: requests allowed"),
               mLimiteByUserRequestsAllowedPerMethod,
               QStringLiteral("DDP_Rate_Limit_User_By_Method_Requests_Allowed"));

    mLimiteByUserIntervalTimePerMethod->setObjectName(QStringLiteral("mLimiteByUserIntervalTimePerMethod"));
    mLimiteByUserIntervalTimePerMethod->setMaximum(9999999);
    addSpinbox(i18n("Limit by User: interval time"), mLimiteByUserIntervalTimePerMethod, QStringLiteral("DDP_Rate_Limit_User_By_Method_Interval_Time"));

    mLimitByConnectionPerMethod->setObjectName(QStringLiteral("mLimitByConnectionPerMethod"));
    mMainLayout->addWidget(mLimitByConnectionPerMethod);
    connectCheckBox(mLimitByConnectionPerMethod, QStringLiteral("DDP_Rate_Limit_Connection_By_Method_Enabled"));

    mLimiteByConnectionRequestsAllowedPerMethod->setObjectName(QStringLiteral("mLimiteByConnectionRequestsAllowedPerMethod"));
    mLimiteByConnectionRequestsAllowedPerMethod->setMaximum(9999999);
    addSpinbox(i18n("Limit by User: requests allowed"),
               mLimiteByConnectionRequestsAllowedPerMethod,
               QStringLiteral("DDP_Rate_Limit_Connection_By_Method_Requests_Allowed"));

    mLimiteByConnectionIntervalTimePerMethod->setObjectName(QStringLiteral("mLimiteByConnectionIntervalTimePerMethod"));
    mLimiteByConnectionIntervalTimePerMethod->setMaximum(9999999);
    addSpinbox(i18n("Limit by User: interval time"),
               mLimiteByConnectionIntervalTimePerMethod,
               QStringLiteral("DDP_Rate_Limit_Connection_By_Method_Interval_Time"));
}

RateLimiterWidget::~RateLimiterWidget() = default;

void RateLimiterWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnableRateLimiter, mapSettings, true);
    initializeWidget(mLimitByIP, mapSettings, true);
    initializeWidget(mLimitByUser, mapSettings, true);
    initializeWidget(mLimitByConnection, mapSettings, true);
    initializeWidget(mLimitByUserPerMethod, mapSettings, true);
    initializeWidget(mLimitByConnectionPerMethod, mapSettings, true);

    initializeWidget(mLimiteByIpRequestsAllowed, mapSettings, 120000);
    initializeWidget(mLimiteByIpIntervalTime, mapSettings, 60000);
    initializeWidget(mLimiteByUserRequestsAllowed, mapSettings, 1200);
    initializeWidget(mLimiteByUserIntervalTime, mapSettings, 60000);
    initializeWidget(mLimiteByConnectionRequestsAllowed, mapSettings, 600);
    initializeWidget(mLimiteByConnectionIntervalTime, mapSettings, 60000);

    initializeWidget(mLimiteByUserRequestsAllowedPerMethod, mapSettings, 20);
    initializeWidget(mLimiteByUserIntervalTimePerMethod, mapSettings, 10000);
    initializeWidget(mLimiteByConnectionRequestsAllowedPerMethod, mapSettings, 10);
    initializeWidget(mLimiteByConnectionIntervalTimePerMethod, mapSettings, 10000);
}
