/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"

#include <QTabWidget>
class ConfigureAccessibilityWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureAccessibilityTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ConfigureAccessibilityTabWidget(QWidget *parent = nullptr);
    ~ConfigureAccessibilityTabWidget() override;

    void save();
    void load();
    void restoreToDefaults();

private:
    ConfigureAccessibilityWidget *const mConfigureAccessibilityWidget;
};
