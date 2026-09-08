/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showstarredmessagesdialog.h"

#include "rocketchataccount.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QWindow>
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

ShowStarredMessagesDialog::~ShowStarredMessagesDialog()
{
    writeConfig();
}

void ShowStarredMessagesDialog::readConfig()
{
#if TEXTADDONSWIDGETS_VERSION >= QT_VERSION_CHECK(2, 1, 49)
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myShowStarredMessagesDialogGroupName), QSize(800, 600));
#else
    create(); // ensure a window is created
    TextAddonsWidgets::LoadDialogSizeUtils::loadDialogSizeScaled(this, QLatin1StringView(myShowStarredMessagesDialogGroupName), 800, 600);
#endif
}

void ShowStarredMessagesDialog::writeConfig()
{
#if TEXTADDONSWIDGETS_VERSION < QT_VERSION_CHECK(2, 1, 49)
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowStarredMessagesDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
#endif
}

#include "moc_showstarredmessagesdialog.cpp"
