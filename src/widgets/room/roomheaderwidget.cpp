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

#include "roomheaderwidget.h"
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QLabel>

RoomHeaderWidget::RoomHeaderWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mRoomName = new QLabel(this);
    mRoomName->setObjectName(QStringLiteral("mRoomName"));
    mainLayout->addWidget(mRoomName);
    mRoomName->setVisible(false);

    mTopic = new QLabel(this);
    mTopic->setObjectName(QStringLiteral("mTopic"));
    mainLayout->addWidget(mTopic);
    mTopic->setVisible(false);

    mAnnouncement = new QLabel(this);
    mAnnouncement->setObjectName(QStringLiteral("mAnnouncement"));
    mainLayout->addWidget(mAnnouncement);
    mAnnouncement->setVisible(false);

    mDescription = new QLabel(this);
    mDescription->setObjectName(QStringLiteral("mDescription"));
    mainLayout->addWidget(mDescription);
    mDescription->setVisible(false);
}

RoomHeaderWidget::~RoomHeaderWidget()
{
}

void RoomHeaderWidget::setRoomName(const QString &name)
{
    mRoomName->setText(name);
    mRoomName->setVisible(!name.isEmpty());
}

QString RoomHeaderWidget::roomName() const
{
    return mRoomName->text();
}
