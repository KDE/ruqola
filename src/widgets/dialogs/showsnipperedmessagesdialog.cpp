/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showsnipperedmessagesdialog.h"
#include "rocketchataccount.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QWindow>
namespace
{
static const char myShowSnipperedMessagesDialogGroupName[] = "ShowSnipperedMessagesDialog";
}

ShowSnipperedMessagesDialog::ShowSnipperedMessagesDialog(RocketChatAccount *account, QWidget *parent)
    : ShowListMessageBaseDialog(account, parent)
{
    setWindowTitle(i18nc("@title:window", "Show Starred Messages - %1", account ? account->accountName() : QStringLiteral("AccountName")));
    readConfig();
}

ShowSnipperedMessagesDialog::~ShowSnipperedMessagesDialog()
{
    writeConfig();
}

void ShowSnipperedMessagesDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowSnipperedMessagesDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ShowSnipperedMessagesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowSnipperedMessagesDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}
