/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QTextBrowser;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsVersionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsVersionWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplicationsSettingsVersionWidget() override;

    void setApplicationId(const QByteArray &appId);

protected:
    void showEvent(QShowEvent *event) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void initialize();
    LIBRUQOLAWIDGETS_NO_EXPORT void generateInfo(const QJsonObject &obj);
    bool mWasInitialized = false;
    QByteArray mAppId;
    QTextBrowser *const mTextBrowser;
    RocketChatAccount *const mRocketChatAccount;
};
