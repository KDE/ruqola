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
class ApplicationsSettingsListView;
class QStackedWidget;
class AppsCountInfoWidget;
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
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFilterChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSortingChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRefreshListApplications();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRefreshCountApplications();
    RocketChatAccount *const mCurrentRocketChatAccount;
    ApplicationsSettingsSearchWidget *const mApplicationsSettingsSearchWidget;
    ApplicationsSettingsListView *const mApplicationsSettingsListView;
    ApplicationsSettingsInProgressWidget *const mApplicationsSettingsInProgressWidget;
    AppsCountInfoWidget *const mAppsCountInfoWidget;
    QStackedWidget *const mStackedWidget;
    QWidget *const mWidgetListView;
};
