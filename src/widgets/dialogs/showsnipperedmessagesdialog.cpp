/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showsnipperedmessagesdialog.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>

namespace
{
static const char myShowSnipperedMessagesDialogGroupName[] = "ShowSnipperedMessagesDialog";
}

ShowSnipperedMessagesDialog::ShowSnipperedMessagesDialog(RocketChatAccount *account, QWidget *parent)
    : ShowListMessageBaseDialog(account, parent)
{
    setWindowTitle(i18nc("@title:window", "Show Snippered Messages"));
    readConfig();
}

ShowSnipperedMessagesDialog::~ShowSnipperedMessagesDialog()
{
    writeConfig();
}

void ShowSnipperedMessagesDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowSnipperedMessagesDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ShowSnipperedMessagesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowSnipperedMessagesDialogGroupName);
    group.writeEntry("Size", size());
}
