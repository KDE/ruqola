/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "grabscreenplugintoolconfigurewidget.h"
#include "grabscreenplugintoolconfig.h"
#include <KLocalization>
#include <KLocalizedString>
#include <QFormLayout>
#include <QSpinBox>

GrabScreenPluginToolConfigureWidget::GrabScreenPluginToolConfigureWidget(QWidget *parent)
    : PluginToolConfigureWidget(parent)
    , mDelay(new QSpinBox(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mDelay->setObjectName(QStringLiteral("mDelay"));
    mainLayout->addRow(i18n("Delay:"), mDelay);
    KLocalization::setupSpinBoxFormatString(mDelay, ki18ncp("@item %v is a number", "%v second", "%v seconds"));
}

GrabScreenPluginToolConfigureWidget::~GrabScreenPluginToolConfigureWidget() = default;

void GrabScreenPluginToolConfigureWidget::saveSettings()
{
    GrabScreenPluginToolConfig::self()->setDelay(mDelay->value() * 1000);
    GrabScreenPluginToolConfig::self()->save();
}

void GrabScreenPluginToolConfigureWidget::loadSettings()
{
    mDelay->setValue(GrabScreenPluginToolConfig::self()->delay() / 1000); // =>second
}

#include "moc_grabscreenplugintoolconfigurewidget.cpp"
