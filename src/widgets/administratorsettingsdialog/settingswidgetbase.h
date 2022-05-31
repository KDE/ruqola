/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QScrollArea>
class RocketChatAccount;
class QCheckBox;
class QSpinBox;
class QFormLayout;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SettingsWidgetBase : public QScrollArea
{
    Q_OBJECT
public:
    explicit SettingsWidgetBase(RocketChatAccount *account, QWidget *parent = nullptr);
    ~SettingsWidgetBase() override;

    virtual void initialize() = 0;

protected:
    void connectCheckBox(QCheckBox *checkBox, const QString &variable);
    QWidget *const mCurrentWidget;

    void addSpinbox(const QString &labelStr, QSpinBox *spinBox, const QString &variable);
    QFormLayout *mMainLayout = nullptr;

private:
    void updateSettings(const QString &settingName, const QVariant &value);
    void slotAdminSettingsDone(const QJsonObject &obj);
    RocketChatAccount *const mAccount;
};
