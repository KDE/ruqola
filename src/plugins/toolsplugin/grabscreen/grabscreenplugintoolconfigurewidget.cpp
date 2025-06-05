/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "grabscreenplugintoolconfigurewidget.h"
#include <QVBoxLayout>

GrabScreenPluginToolConfigureWidget::GrabScreenPluginToolConfigureWidget(QWidget *parent)
    : PluginToolConfigureWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    // TODO add more
}

GrabScreenPluginToolConfigureWidget::~GrabScreenPluginToolConfigureWidget() = default;

void GrabScreenPluginToolConfigureWidget::save()
{
    // TODO
}

void GrabScreenPluginToolConfigureWidget::read()
{
    // TODO
}

#include "moc_grabscreenplugintoolconfigurewidget.cpp"
