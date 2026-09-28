/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showstarredmessagesdialog.h"

#include "rocketchataccount.h"
#include <KLocalizedString>
#include <TextAddonsWidgets/LoadDialogSizeUtils>
namespace
{
const char myShowStarredMessagesDialogGroupName[] = "ShowStarredMessagesDialog";
}

using namespace Qt::Literals::StringLiterals;
ShowStarredMessagesDialog::ShowStarredMessagesDialog(RocketChatAccount *account, QWidget *parent)
    : ShowListMessageBaseDialog(account, parent)
{
    setWindowTitle(i18nc("@title:window", "Show Starred Messages - %1", account ? account->accountName() : u"AccountName"_s));
    readConfig();
}

ShowStarredMessagesDialog::~ShowStarredMessagesDialog() = default;

void ShowStarredMessagesDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myShowStarredMessagesDialogGroupName), QSize(800, 600));
}

#include "moc_showstarredmessagesdialog.cpp"
