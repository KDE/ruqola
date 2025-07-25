/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QLineEdit;
class QSpinBox;
class QComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT LogsSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit LogsSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~LogsSettingsWidget() override;

    void initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings) override;

private:
    QLineEdit *const mLogExceptionsChannel;
    QComboBox *const mLogLevel;
    QSpinBox *const mLogViewLimit;
    QCheckBox *const mTraceMethodCalls;
    QLineEdit *const mTraceMethodFilter;
    QCheckBox *const mTraceSubscriptionCalls;
    QLineEdit *const mTraceSubscriptionFilter;
};
