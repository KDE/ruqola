/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QSpinBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MobileSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit MobileSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MobileSettingsWidget() override;

    void initialize(const QMap<QString, QVariant> &mapSettings) override;

private:
    QCheckBox *const mAllowSaveMediaGallery;
    QCheckBox *const mForceScreenLock;
    QSpinBox *const mForceScreenLockAfter;
};
