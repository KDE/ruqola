/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showstarredmessagesdialog.h"
#include "rocketchataccount.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>

namespace
{
static const char myShowStarredMessagesDialogGroupName[] = "ShowStarredMessagesDialog";
}

ShowStarredMessagesDialog::ShowStarredMessagesDialog(RocketChatAccount *account, QWidget *parent)
    : ShowListMessageBaseDialog(account, parent)
{
    setWindowTitle(i18nc("@title:window", "Show Starred Messages - %1", account ? account->accountName() : QStringLiteral("AccountName")));
    readConfig();
}

ShowStarredMessagesDialog::~ShowStarredMessagesDialog()
{
    writeConfig();
}

void ShowStarredMessagesDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowStarredMessagesDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ShowStarredMessagesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowStarredMessagesDialogGroupName);
    group.writeEntry("Size", size());
}
