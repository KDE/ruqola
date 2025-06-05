/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "grabscreenplugintoolconfiguredialog.h"
#include "grabscreenplugintoolconfigurewidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

GrabScreenPluginToolConfigureDialog::GrabScreenPluginToolConfigureDialog(QWidget *parent)
    : QDialog(parent)
    , mGrabScreenPluginToolConfigureWidget(new GrabScreenPluginToolConfigureWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Configure Grab Screen"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mGrabScreenPluginToolConfigureWidget->setObjectName(QStringLiteral("mGrabScreenPluginToolConfigureWidget"));
    mainLayout->addWidget(mGrabScreenPluginToolConfigureWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    mGrabScreenPluginToolConfigureWidget->loadSettings();
    connect(button, &QDialogButtonBox::rejected, this, &GrabScreenPluginToolConfigureDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &GrabScreenPluginToolConfigureDialog::slotAccepted);
}

GrabScreenPluginToolConfigureDialog::~GrabScreenPluginToolConfigureDialog() = default;

void GrabScreenPluginToolConfigureDialog::slotAccepted()
{
    mGrabScreenPluginToolConfigureWidget->saveSettings();
    accept();
}

#include "moc_grabscreenplugintoolconfiguredialog.cpp"
