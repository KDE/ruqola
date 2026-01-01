/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class ApplicationsSettingsDescriptionWidget;
class ApplicationsSettingsVersionWidget;
class ApplicationsSettingsLogsWidget;
class ApplicationsSettingsSettingsWidget;
class QTabWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsDescriptionTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsDescriptionTabWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsDescriptionTabWidget() override;
    void setDescription(const QString &desc);

    void setApplicationId(const QByteArray &appId);

    void setShowLogAndSettingsInfo(bool state);

    void setShowVersionInfo(bool state);

private:
    QTabWidget *const mTabWidget;
    ApplicationsSettingsDescriptionWidget *const mDescriptionWidget;
    ApplicationsSettingsVersionWidget *const mVersionWidget;
    ApplicationsSettingsLogsWidget *const mLogsWidget;
    ApplicationsSettingsSettingsWidget *const mSettingsWidget;
    RocketChatAccount *const mRocketChatAccount;
};
