/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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
#include "room.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

ChannelInfoDialog::ChannelInfoDialog(QWidget *parent)
    : QDialog(parent)
    , mChannelInfoWidget(new ChannelInfoWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Channel Info"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mChannelInfoWidget->setObjectName(QStringLiteral("mChannelInfoWidget"));
    mainLayout->addWidget(mChannelInfoWidget);
    mChannelInfoWidget->updateUiFromPermission();

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ChannelInfoDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ChannelInfoDialog::accept);
    mainLayout->addWidget(buttonBox);
    resize(600, 400);
    connect(mChannelInfoWidget, &ChannelInfoWidget::channelDeleted, this, &ChannelInfoDialog::close);
    connect(mChannelInfoWidget, &ChannelInfoWidget::fnameChanged, this, &ChannelInfoDialog::slotFnameChanged);
}

ChannelInfoDialog::~ChannelInfoDialog()
{
}

RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo ChannelInfoDialog::saveRoomSettingsInfo() const
{
    // TODO
    return {};
}
// TODO return saveRoomsSettingsInfo

void ChannelInfoDialog::setRoom(Room *room)
{
    slotFnameChanged(room->displayFName());
    mChannelInfoWidget->setRoom(room);
}

void ChannelInfoDialog::slotFnameChanged(const QString &fname)
{
    setWindowTitle(i18nc("@title:window", "Channel Info about \'%1\'", fname));
}
