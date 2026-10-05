/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureaccessibilitytabwidget.h"

ConfigureAccessibilityTabWidget::ConfigureAccessibilityTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setTabBarAutoHide(true);
}

ConfigureAccessibilityTabWidget::~ConfigureAccessibilityTabWidget() = default;

void ConfigureAccessibilityTabWidget::save()
{
}

void ConfigureAccessibilityTabWidget::load()
{
}

void ConfigureAccessibilityTabWidget::restoreToDefaults()
{
}

#include "moc_configureaccessibilitytabwidget.cpp"
