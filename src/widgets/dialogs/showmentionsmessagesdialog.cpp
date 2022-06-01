/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showmentionsmessagesdialog.h"
#include "rocketchataccount.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>

namespace
{
static const char myShowMentionsMessagesDialogGroupName[] = "ShowMentionsMessagesDialog";
}

ShowMentionsMessagesDialog::ShowMentionsMessagesDialog(RocketChatAccount *account, QWidget *parent)
    : ShowListMessageBaseDialog(account, parent)
{
    setWindowTitle(i18nc("@title:window", "Show Mentions Messages - %1", account ? account->accountName() : QStringLiteral("AccountName")));
    readConfig();
}

ShowMentionsMessagesDialog::~ShowMentionsMessagesDialog()
{
    writeConfig();
}

void ShowMentionsMessagesDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowMentionsMessagesDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ShowMentionsMessagesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowMentionsMessagesDialogGroupName);
    group.writeEntry("Size", size());
}
