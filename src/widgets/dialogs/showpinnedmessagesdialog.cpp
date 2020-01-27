/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "showpinnedmessagesdialog.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>

namespace {
static const char myConfigGroupName[] = "ShowPinnedMessagesDialog";
}

ShowPinnedMessagesDialog::ShowPinnedMessagesDialog(QWidget *parent)
    : ShowListMessageBaseDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Show Pinned Messages"));
    readConfig();
    connect(this, &ShowPinnedMessagesDialog::loadMoreElements, this, &ShowPinnedMessagesDialog::slotLoadMorePinnedMessages);
}

ShowPinnedMessagesDialog::~ShowPinnedMessagesDialog()
{
    writeConfig();
}

void ShowPinnedMessagesDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ShowPinnedMessagesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
}

void ShowPinnedMessagesDialog::slotLoadMorePinnedMessages()
{
    //Ruqola::self()->rocketChatAccount()->loadMoreListMessages(roomId);
}
