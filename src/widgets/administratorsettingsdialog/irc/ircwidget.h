/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QSpinBox;
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT IrcWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit IrcWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~IrcWidget() override;

    void initialize(const QMap<QString, QVariant> &mapSettings) override;

private:
    QSpinBox *const mCacheLimitOutBoundMessage;
    QLineEdit *const mLoginSuccessful;
};
