/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showstarredmessagesdialog.h"
#include "rocketchataccount.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QWindow>
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
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowStarredMessagesDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ShowStarredMessagesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowStarredMessagesDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}
