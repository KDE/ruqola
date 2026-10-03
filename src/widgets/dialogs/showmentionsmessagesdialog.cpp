/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showmentionsmessagesdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "rocketchataccount.h"
#include <KLocalizedString>
#include <TextAddonsWidgets/LoadDialogSizeUtils>
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

ShowMentionsMessagesDialog::~ShowMentionsMessagesDialog() = default;

void ShowMentionsMessagesDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myShowMentionsMessagesDialogGroupName), QSize(800, 600));
}

#include "moc_showmentionsmessagesdialog.cpp"
