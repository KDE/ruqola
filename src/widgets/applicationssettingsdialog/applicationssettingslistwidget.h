/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "applicationssettingsdialog/applicationssettingssearchwidget.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class ApplicationsSettingsSearchWidget;
class ApplicationsSettingsListView;
class AppsCountInfoWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsListWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsListWidget() override;

    void setFeature(ApplicationsSettingsSearchWidget::Feature feature);

private:
    void slotFilterChanged();
    void slotSortingChanged();
    void slotRefreshCountApplications();
    void slotRefreshListApplications();

    ApplicationsSettingsSearchWidget *const mApplicationsSettingsSearchWidget;
    ApplicationsSettingsListView *const mApplicationsSettingsListView;
    AppsCountInfoWidget *const mAppsCountInfoWidget;
    RocketChatAccount *const mCurrentRocketChatAccount;
};
