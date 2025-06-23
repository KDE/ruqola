/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showpinnedmessagesdialog.h"
using namespace Qt::Literals::StringLiterals;

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
    setWindowTitle(i18nc("@title:window", "Show Pinned Messages - %1", account ? account->accountName() : u"AccountName"_s));
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
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowPinnedMessagesDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ShowPinnedMessagesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowPinnedMessagesDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_showpinnedmessagesdialog.cpp"
