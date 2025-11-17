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
class ApplicationsSettingsDescriptionTabWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsListWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsListWidget() override;

    void setFeature(ApplicationsSettingsSearchWidget::Feature feature);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFilterChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSortingChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRefreshCountApplications();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRefreshListApplications();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUpdateDescription(const QModelIndex &index);

    ApplicationsSettingsSearchWidget *const mApplicationsSettingsSearchWidget;
    ApplicationsSettingsListView *const mApplicationsSettingsListView;
    AppsCountInfoWidget *const mAppsCountInfoWidget;
    RocketChatAccount *const mCurrentRocketChatAccount;
    ApplicationsSettingsDescriptionTabWidget *const mApplicationsSettingsDescriptionTabWidget;
};
