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
#if HAVE_TEXT_TO_SPEECH
    addTab(mConfigureAccessibilityWidget, i18n("Text to Speech"));
#endif
}

ConfigureAccessibilityTabWidget::~ConfigureAccessibilityTabWidget() = default;

void ConfigureAccessibilityTabWidget::save()
{
#if HAVE_TEXT_TO_SPEECH
    mConfigureAccessibilityWidget->save();
#endif
}

void ConfigureAccessibilityTabWidget::load()
{
#if HAVE_TEXT_TO_SPEECH
    mConfigureAccessibilityWidget->load();
#endif
}

void ConfigureAccessibilityTabWidget::restoreToDefaults()
{
#if HAVE_TEXT_TO_SPEECH
    mConfigureAccessibilityWidget->restoreToDefaults();
#endif
}

#include "moc_configureaccessibilitytabwidget.cpp"
