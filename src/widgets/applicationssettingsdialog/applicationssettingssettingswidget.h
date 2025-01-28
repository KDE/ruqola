/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "apps/applicationssettingssettingsinfo.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QVBoxLayout;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsSettingsWidget() override;

    void setApplicationId(const QByteArray &appId);

protected:
    void showEvent(QShowEvent *event) override;

Q_SIGNALS:
    void dataChanged(bool status);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void generateSettings(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void initialize();
    LIBRUQOLAWIDGETS_NO_EXPORT void generateSettingsWidgets(const QList<ApplicationsSettingsSettingsInfo> &infos);
    LIBRUQOLAWIDGETS_NO_EXPORT void addBooleanSettings(const ApplicationsSettingsSettingsInfo &info);
    LIBRUQOLAWIDGETS_NO_EXPORT void addStringSettings(const ApplicationsSettingsSettingsInfo &info);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString getTranslatedIdentifier(const QString &lang, const QString &id) const;
    bool mWasInitialized = false;
    QByteArray mAppId;
    RocketChatAccount *const mRocketChatAccount;
    QVBoxLayout *const mMainLayout;
};
