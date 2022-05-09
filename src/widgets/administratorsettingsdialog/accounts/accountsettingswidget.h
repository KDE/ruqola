/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QCheckBox;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AccountSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AccountSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AccountSettingsWidget() override;

    void initialize();

private:
    void updateSettings(const QString &settingName, bool value);
    void slotAdminSettingsDone(const QJsonObject &obj);
    QCheckBox *const mAllowChangeName;
    QCheckBox *const mAllowChangeUserProfile;
    QCheckBox *const mAllowChangeUserAvatar;
    QCheckBox *const mAllowChangeEmail;
    QCheckBox *const mAllowChangePassword;
    RocketChatAccount *const mAccount;
};
