/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT IrcFederationWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit IrcFederationWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~IrcFederationWidget() override;

    void initialize(const QMap<QString, QVariant> &mapSettings) override;

private:
    QCheckBox *const mEnabled;
    QComboBox *const mProtocol;
    QLineEdit *const mHost;
    QSpinBox *const mPort;
    QLineEdit *const mName;
    QLineEdit *const mDescription;
};
