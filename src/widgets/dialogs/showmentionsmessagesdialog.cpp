/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showmentionsmessagesdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "rocketchataccount.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QWindow>
namespace
{
const char myShowMentionsMessagesDialogGroupName[] = "ShowMentionsMessagesDialog";
}

ShowMentionsMessagesDialog::ShowMentionsMessagesDialog(RocketChatAccount *account, QWidget *parent)
    : ShowListMessageBaseDialog(account, parent)
{
    setWindowTitle(i18nc("@title:window", "Show Mentions Messages - %1", account ? account->accountName() : u"AccountName"_s));
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
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowMentionsMessagesDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ShowMentionsMessagesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowMentionsMessagesDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_showmentionsmessagesdialog.cpp"
