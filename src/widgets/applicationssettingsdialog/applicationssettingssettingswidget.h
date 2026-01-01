/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "apps/applicationssettingssettingsinfo.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QVBoxLayout;
class RocketChatAccount;
class ApplictionSettingsCustomWidgetsBase;
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
    void resetValue();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void generateSettings(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void initialize();
    LIBRUQOLAWIDGETS_NO_EXPORT void generateSettingsWidgets(const QList<ApplicationsSettingsSettingsInfo> &infos);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT ApplictionSettingsCustomWidgetsBase *addBooleanSettings(const ApplicationsSettingsSettingsInfo &info);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT ApplictionSettingsCustomWidgetsBase *addStringSettings(const ApplicationsSettingsSettingsInfo &info);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT ApplictionSettingsCustomWidgetsBase *addIntSettings(const ApplicationsSettingsSettingsInfo &info);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT ApplictionSettingsCustomWidgetsBase *addSelectSettings(const ApplicationsSettingsSettingsInfo &info);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString getTranslatedIdentifier(const QString &lang, const QString &id) const;
    QList<ApplicationsSettingsSettingsInfo> mSettingsInfo;
    bool mWasInitialized = false;
    QByteArray mAppId;
    RocketChatAccount *const mRocketChatAccount;
    QVBoxLayout *const mMainLayout;
    QList<ApplictionSettingsCustomWidgetsBase *> mCustomWidgets;
};
