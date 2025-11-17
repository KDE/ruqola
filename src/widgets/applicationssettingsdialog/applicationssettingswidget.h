/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "applicationssettingssearchwidget.h"
#include "libruqolawidgets_private_export.h"
#include "rocketchataccount.h"
#include <QWidget>
class RocketChatAccount;
class ApplicationsSettingsListWidget;
class QStackedWidget;
class ApplicationsSettingsInProgressWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsWidget() override;

    void setFeature(ApplicationsSettingsSearchWidget::Feature feature);

    void initialize();

private:
    RocketChatAccount *const mCurrentRocketChatAccount;
    ApplicationsSettingsListWidget *const mApplicationsSettingsListWidget;
    ApplicationsSettingsInProgressWidget *const mApplicationsSettingsInProgressWidget;
    QStackedWidget *const mStackedWidget;
};
