/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class RocketChatAccount;
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SettingsWidgetBase : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWidgetBase(RocketChatAccount *account, QWidget *parent = nullptr);
    ~SettingsWidgetBase() override;

protected:
    void updateSettings(const QString &settingName, bool value);

    void connectCheckBox(QCheckBox *checkBox, const QString &variable);

private:
    void slotAdminSettingsDone(const QJsonObject &obj);
    RocketChatAccount *const mAccount;
};
