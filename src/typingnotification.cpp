/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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

#include "typingnotification.h"
#include <QTimer>

TypingNotification::TypingNotification(QObject *parent)
    : QObject(parent)
    , mTypingInprogress(false)
{
    mTimer = new QTimer(this);
    mTimer->setObjectName(QStringLiteral("typingnotificationtimer"));
    mTimer->setInterval(2000);
    mTimer->setSingleShot(true);
    connect(mTimer, &QTimer::timeout, this, &TypingNotification::slotTimeout);
}

TypingNotification::~TypingNotification()
{
    if (mTimer->isActive()) {
        mTimer->stop();
    }
}

void TypingNotification::setText(const QString &roomId, const QString &str)
{
    if (mTypingInprogress) {
        if (str.isEmpty()) {
            if (mTimer->isActive()) {
                mTimer->stop();
            }
            mTypingInprogress = false;
            //1) Send info about typing.
            informTypingStatus(roomId, false);
        } else {
            if (mRoomId != roomId) {
                //We changed room.
                //1) stop typing in old room
                informTypingStatus(roomId, false);
            }

            //2) start info about typing in new room.
            informTypingStatus(mRoomId, true);

            //3) restart timer.
            mTimer->start();
        }
    } else {
        mTypingInprogress = true;
        informTypingStatus(roomId, true);
        //Send info about typing.
        //Restart timer.
        mTimer->start();
    }
    mRoomId = roomId;
}

void TypingNotification::slotTimeout()
{
    mTypingInprogress = false;
    //Send info about stopping typing.
    informTypingStatus(mRoomId, false);
}
