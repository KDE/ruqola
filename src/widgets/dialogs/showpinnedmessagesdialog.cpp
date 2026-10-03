/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showpinnedmessagesdialog.h"

#include "rocketchataccount.h"

#include <KLocalizedString>
#include <TextAddonsWidgets/LoadDialogSizeUtils>
namespace
{
const char myShowPinnedMessagesDialogGroupName[] = "ShowPinnedMessagesDialog";
}

using namespace Qt::Literals::StringLiterals;
ShowPinnedMessagesDialog::ShowPinnedMessagesDialog(RocketChatAccount *account, QWidget *parent)
    : ShowListMessageBaseDialog(account, parent)
{
    setWindowTitle(i18nc("@title:window", "Show Pinned Messages - %1", account ? account->accountName() : u"AccountName"_s));
    readConfig();
}

ShowPinnedMessagesDialog::~ShowPinnedMessagesDialog() = default;

void ShowPinnedMessagesDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myShowPinnedMessagesDialogGroupName), QSize(800, 600));
}

#include "moc_showpinnedmessagesdialog.cpp"
