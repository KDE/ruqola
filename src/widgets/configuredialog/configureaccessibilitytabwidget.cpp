/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureaccessibilitytabwidget.h"
#include "configuredialog/configureaccessibilitywidget.h"
#include <KLocalizedString>

ConfigureAccessibilityTabWidget::ConfigureAccessibilityTabWidget(QWidget *parent)
    : QTabWidget(parent)
    , mConfigureAccessibilityWidget(new ConfigureAccessibilityWidget(this))
{
    setTabBarAutoHide(true);
    addTab(mConfigureAccessibilityWidget, i18n("Text to Speech"));
}

ConfigureAccessibilityTabWidget::~ConfigureAccessibilityTabWidget() = default;

void ConfigureAccessibilityTabWidget::save()
{
    mConfigureAccessibilityWidget->save();
}

void ConfigureAccessibilityTabWidget::load()
{
    mConfigureAccessibilityWidget->load();
}

void ConfigureAccessibilityTabWidget::restoreToDefaults()
{
    mConfigureAccessibilityWidget->restoreToDefaults();
}

#include "moc_configureaccessibilitytabwidget.cpp"
