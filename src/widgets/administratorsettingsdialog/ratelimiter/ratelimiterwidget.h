/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QSpinBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RateLimiterWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit RateLimiterWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~RateLimiterWidget() override;
    void initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings) override;

private:
    QCheckBox *const mEnableRateLimiter;

    QCheckBox *const mLimitByIP;
    QSpinBox *const mLimiteByIpRequestsAllowed;
    QSpinBox *const mLimiteByIpIntervalTime;

    QCheckBox *const mLimitByUser;
    QSpinBox *const mLimiteByUserRequestsAllowed;
    QSpinBox *const mLimiteByUserIntervalTime;

    QCheckBox *const mLimitByConnection;
    QSpinBox *const mLimiteByConnectionRequestsAllowed;
    QSpinBox *const mLimiteByConnectionIntervalTime;

    QCheckBox *const mLimitByUserPerMethod;
    QSpinBox *const mLimiteByUserRequestsAllowedPerMethod;
    QSpinBox *const mLimiteByUserIntervalTimePerMethod;

    QCheckBox *const mLimitByConnectionPerMethod;
    QSpinBox *const mLimiteByConnectionRequestsAllowedPerMethod;
    QSpinBox *const mLimiteByConnectionIntervalTimePerMethod;
};
