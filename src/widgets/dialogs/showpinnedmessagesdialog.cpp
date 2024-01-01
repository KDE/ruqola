/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showpinnedmessagesdialog.h"
#include "rocketchataccount.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QWindow>
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
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myShowPinnedMessagesDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ShowPinnedMessagesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myShowPinnedMessagesDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_showpinnedmessagesdialog.cpp"
