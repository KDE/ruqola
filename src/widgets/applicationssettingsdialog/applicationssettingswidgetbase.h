/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "applicationssettingssearchwidget.h"
#include "libruqolawidgets_private_export.h"
#include "rocketchataccount.h"
#include <QWidget>
class RocketChatAccount;
class ApplicationsSettingsListViewBase;
class QStackedWidget;
class AppsCountInfoWidget;
class ApplicationsSettingsInProgressWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsWidgetBase : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsWidgetBase(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsWidgetBase() override;

    void setFeature(ApplicationsSettingsSearchWidget::Feature feature);

    void initialize();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFilterChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSortingChanged();
    RocketChatAccount *const mCurrentRocketChatAccount;
    ApplicationsSettingsSearchWidget *const mApplicationsSettingsSearchWidget;
    ApplicationsSettingsListViewBase *const mApplicationsSettingsListView;
    ApplicationsSettingsInProgressWidget *const mApplicationsSettingsInProgressWidget;
    AppsCountInfoWidget *const mAppsCountInfoWidget;
    QStackedWidget *const mStackedWidget;
    QWidget *const mWidgetListView;
};
