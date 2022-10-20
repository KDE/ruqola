/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT LogsSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit LogsSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~LogsSettingsWidget() override;

    void initialize(const QMap<QString, QVariant> &mapSettings) override;

private:
    QLineEdit *const mLogExceptionsChannel;
};
