/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "grabscreenplugintoolconfiguredialog.h"
#include "grabscreenplugintoolconfigurewidget.h"

#include <QVBoxLayout>

GrabScreenPluginToolConfigureDialog::GrabScreenPluginToolConfigureDialog(QWidget *parent)
    : QDialog(parent)
    , mGrabScreenPluginToolConfigureWidget(new GrabScreenPluginToolConfigureWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mGrabScreenPluginToolConfigureWidget->setObjectName(QStringLiteral("mGrabScreenPluginToolConfigureWidget"));
    mainLayout->addWidget(mGrabScreenPluginToolConfigureWidget);
}

GrabScreenPluginToolConfigureDialog::~GrabScreenPluginToolConfigureDialog() = default;
