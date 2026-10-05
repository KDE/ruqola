/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureaccessibilitytabwidget.h"
#include "config-ruqola.h"
#if HAVE_TEXT_TO_SPEECH
#include "configureaccessibilitywidget.h"
#endif
#include <KLocalizedString>

ConfigureAccessibilityTabWidget::ConfigureAccessibilityTabWidget(QWidget *parent)
    : QTabWidget(parent)
#if HAVE_TEXT_TO_SPEECH
    , mConfigureAccessibilityWidget(new ConfigureAccessibilityWidget(this))
#endif
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
