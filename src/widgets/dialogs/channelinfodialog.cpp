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

#include "channelinfodialog.h"
#include "channelinfowidget.h"
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <KLocalizedString>
#include <KMessageBox>

ChannelInfoDialog::ChannelInfoDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Channel Info"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mChannelInfoWidget = new ChannelInfoWidget(this);
    mChannelInfoWidget->setObjectName(QStringLiteral("mChannelInfoWidget"));
    mainLayout->addWidget(mChannelInfoWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ChannelInfoDialog::reject);
    mainLayout->addWidget(buttonBox);
    resize(600, 400);
    connect(mChannelInfoWidget, &ChannelInfoWidget::deleteChannel, this, &ChannelInfoDialog::slotDeleteChannel);
}

ChannelInfoDialog::~ChannelInfoDialog()
{
}

void ChannelInfoDialog::setRoomWrapper(RoomWrapper *roomWrapper)
{
    mChannelInfoWidget->setRoomWrapper(roomWrapper);
}

void ChannelInfoDialog::slotDeleteChannel()
{
    if (KMessageBox::Yes == KMessageBox::questionYesNo(this, i18n("Do you want to delete this room?"), i18n("Delete Room"))) {
        //TODO delete it !
        accept();
    }
}
