/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showpinnedmessagesdialog.h"
#include "rocketchataccount.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
namespace
{
static const char myShowPinnedMessagesDialogGroupName[] = "ShowPinnedMessagesDialog";
}

ShowPinnedMessagesDialog::ShowPinnedMessagesDialog(RocketChatAccount *account, QWidget *parent)
    : ShowListMessageBaseDialog(account, parent)
{
    setWindowTitle(i18nc("@title:window", "Show Pinned Messages - %1", account ? account->accountName() : QStringLiteral("AccountName")));
    readConfig();
}

ShowPinnedMessagesDialog::~ShowPinnedMessagesDialog()
{
    writeConfig();
}

void ShowPinnedMessagesDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowPinnedMessagesDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ShowPinnedMessagesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowPinnedMessagesDialogGroupName);
    group.writeEntry("Size", size());
}
