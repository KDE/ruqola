/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "whatsnewdialog.h"

#include <KConfigGroup>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QWindow>
namespace
{
const char myWhatsNewDialogGroupName[] = "WhatsNewDialog";
}
WhatsNewDialog::WhatsNewDialog(QWidget *parent)
    : QDialog(parent)
{
    readConfig();
}

WhatsNewDialog::~WhatsNewDialog()
{
    writeConfig();
}

void WhatsNewDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), myWhatsNewDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void WhatsNewDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myWhatsNewDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}
