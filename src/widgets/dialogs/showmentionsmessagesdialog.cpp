/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showmentionsmessagesdialog.h"
#include "rocketchataccount.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QWindow>
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
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowMentionsMessagesDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ShowMentionsMessagesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowMentionsMessagesDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}
