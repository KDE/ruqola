/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "ratelimiterwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>

RateLimiterWidget::RateLimiterWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mEnableRateLimiter(new QCheckBox(i18nc("@option:check", "Enable Rate Limiter"), this))
    , mLimitByIP(new QCheckBox(i18nc("@option:check", "Limit by IP"), this))
    , mLimiteByIpRequestsAllowed(new QSpinBox(this))
    , mLimiteByIpIntervalTime(new QSpinBox(this))
    , mLimitByUser(new QCheckBox(i18nc("@option:check", "Limit by User"), this))
    , mLimiteByUserRequestsAllowed(new QSpinBox(this))
    , mLimiteByUserIntervalTime(new QSpinBox(this))
    , mLimitByConnection(new QCheckBox(i18nc("@option:check", "Limit by Connection"), this))
    , mLimiteByConnectionRequestsAllowed(new QSpinBox(this))
    , mLimiteByConnectionIntervalTime(new QSpinBox(this))

    , mLimitByUserPerMethod(new QCheckBox(i18nc("@option:check", "Limit by User per Method"), this))
    , mLimiteByUserRequestsAllowedPerMethod(new QSpinBox(this))
    , mLimiteByUserIntervalTimePerMethod(new QSpinBox(this))
    , mLimitByConnectionPerMethod(new QCheckBox(i18nc("@option:check", "Limit by Connection per Method"), this))
    , mLimiteByConnectionRequestsAllowedPerMethod(new QSpinBox(this))
    , mLimiteByConnectionIntervalTimePerMethod(new QSpinBox(this))
{
    auto apiRateLimiterLabel = createBoldLabel(i18n("API Rate Limiter"));
    apiRateLimiterLabel->setObjectName(u"apiRateLimiterLabel"_s);
    mMainLayout->addWidget(apiRateLimiterLabel);

    mEnableRateLimiter->setObjectName(u"mEnableRateLimiter"_s);
    addCheckBox(mEnableRateLimiter, u"API_Enable_Rate_Limiter"_s);

    auto ddpRateLimiterLabel = createBoldLabel(i18n("DDP Rate Limit"));
    ddpRateLimiterLabel->setObjectName(u"ddpRateLimiterLabel"_s);
    mMainLayout->addWidget(ddpRateLimiterLabel);

    mLimitByIP->setObjectName(u"mLimitByIP"_s);
    addCheckBox(mLimitByIP, u"DDP_Rate_Limit_IP_Enabled"_s);

    mLimiteByIpRequestsAllowed->setObjectName(u"mLimiteByIpRequestsAllowed"_s);
    mLimiteByIpRequestsAllowed->setMaximum(9999999);
    addSpinbox(i18n("Limit by IP: interval time"), mLimiteByIpRequestsAllowed, u"DDP_Rate_Limit_IP_Requests_Allowed"_s);

    mLimiteByIpIntervalTime->setObjectName(u"mLimiteByIpIntervalTime"_s);
    mLimiteByIpIntervalTime->setMaximum(9999999);
    addSpinbox(i18n("Limit by IP: requests allowed"), mLimiteByIpIntervalTime, u"DDP_Rate_Limit_IP_Interval_Time"_s);

    mLimitByUser->setObjectName(u"mLimitByUser"_s);
    addCheckBox(mLimitByUser, u"DDP_Rate_Limit_User_Enabled"_s);

    mLimiteByUserRequestsAllowed->setObjectName(u"mLimiteByUserRequestsAllowed"_s);
    mLimiteByUserRequestsAllowed->setMaximum(9999999);
    addSpinbox(i18n("Limit by User: requests allowed"), mLimiteByUserRequestsAllowed, u"DDP_Rate_Limit_User_Requests_Allowed"_s);

    mLimiteByUserIntervalTime->setObjectName(u"mLimiteByUserIntervalTime"_s);
    mLimiteByUserIntervalTime->setMaximum(9999999);
    addSpinbox(i18n("Limit by User: interval time"), mLimiteByUserIntervalTime, u"DDP_Rate_Limit_User_Interval_Time"_s);

    mLimitByConnection->setObjectName(u"mLimitByConnection"_s);
    addCheckBox(mLimitByConnection, u"DDP_Rate_Limit_Connection_Enabled"_s);

    mLimiteByConnectionRequestsAllowed->setObjectName(u"mLimiteByConnectionRequestsAllowed"_s);
    mLimiteByConnectionRequestsAllowed->setMaximum(9999999);
    addSpinbox(i18n("Limit by User: requests allowed"), mLimiteByConnectionRequestsAllowed, u"DDP_Rate_Limit_Connection_Requests_Allowed"_s);

    mLimiteByConnectionIntervalTime->setObjectName(u"mLimiteByConnectionIntervalTime"_s);
    mLimiteByConnectionIntervalTime->setMaximum(9999999);
    addSpinbox(i18n("Limit by User: interval time"), mLimiteByConnectionIntervalTime, u"DDP_Rate_Limit_Connection_Interval_Time"_s);

    mLimitByUserPerMethod->setObjectName(u"mLimitByUserPerMethod"_s);
    addCheckBox(mLimitByUserPerMethod, u"DDP_Rate_Limit_User_By_Method_Enabled"_s);

    mLimiteByUserRequestsAllowedPerMethod->setObjectName(u"mLimiteByUserRequestsAllowedPerMethod"_s);
    mLimiteByUserRequestsAllowedPerMethod->setMaximum(9999999);
    addSpinbox(i18n("Limit by User: requests allowed"), mLimiteByUserRequestsAllowedPerMethod, u"DDP_Rate_Limit_User_By_Method_Requests_Allowed"_s);

    mLimiteByUserIntervalTimePerMethod->setObjectName(u"mLimiteByUserIntervalTimePerMethod"_s);
    mLimiteByUserIntervalTimePerMethod->setMaximum(9999999);
    addSpinbox(i18n("Limit by User: interval time"), mLimiteByUserIntervalTimePerMethod, u"DDP_Rate_Limit_User_By_Method_Interval_Time"_s);

    mLimitByConnectionPerMethod->setObjectName(u"mLimitByConnectionPerMethod"_s);
    addCheckBox(mLimitByConnectionPerMethod, u"DDP_Rate_Limit_Connection_By_Method_Enabled"_s);

    mLimiteByConnectionRequestsAllowedPerMethod->setObjectName(u"mLimiteByConnectionRequestsAllowedPerMethod"_s);
    mLimiteByConnectionRequestsAllowedPerMethod->setMaximum(9999999);
    addSpinbox(i18n("Limit by User: requests allowed"), mLimiteByConnectionRequestsAllowedPerMethod, u"DDP_Rate_Limit_Connection_By_Method_Requests_Allowed"_s);

    mLimiteByConnectionIntervalTimePerMethod->setObjectName(u"mLimiteByConnectionIntervalTimePerMethod"_s);
    mLimiteByConnectionIntervalTimePerMethod->setMaximum(9999999);
    addSpinbox(i18n("Limit by User: interval time"), mLimiteByConnectionIntervalTimePerMethod, u"DDP_Rate_Limit_Connection_By_Method_Interval_Time"_s);
}

RateLimiterWidget::~RateLimiterWidget() = default;

void RateLimiterWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
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

#include "moc_ratelimiterwidget.cpp"
