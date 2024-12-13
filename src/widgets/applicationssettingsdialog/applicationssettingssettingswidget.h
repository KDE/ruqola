/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

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

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void generateSettings(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void initialize();
    bool mWasInitialized = false;
    QByteArray mAppId;
    RocketChatAccount *const mRocketChatAccount;
    QVBoxLayout *const mMainLayout;
};
