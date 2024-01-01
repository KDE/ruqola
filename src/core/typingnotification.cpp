/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "typingnotification.h"
#include <QTimer>

extern LIBRUQOLACORE_TESTS_EXPORT int timerTimeOutValueMs;
LIBRUQOLACORE_TESTS_EXPORT int timerTimeOutValueMs = 2000;

TypingNotification::TypingNotification(QObject *parent)
    : QObject(parent)
    , mTimer(new QTimer(this))
{
    mTimer->setObjectName(QStringLiteral("typingnotificationtimer"));
    mTimer->setInterval(timerTimeOutValueMs);
    mTimer->setSingleShot(true);
    connect(mTimer, &QTimer::timeout, this, &TypingNotification::slotTimeout);
}

TypingNotification::~TypingNotification()
{
    if (mTimer->isActive()) {
        mTimer->stop();
    }
}

void TypingNotification::textNotificationChanged(const QString &roomId, bool emptyString)
{
    if (mTimer->isActive()) {
        mTimer->stop();
    }
    if (mTypingInprogress) {
        if (emptyString) {
            mTypingInprogress = false;
            // 1) Send info about typing.
            Q_EMIT informTypingStatus(roomId, false);
        } else {
            if (mRoomId != roomId) {
                // We changed room.
                // 1) stop typing in old room
                Q_EMIT informTypingStatus(roomId, false);

                // 2) start info about typing in new room.
                Q_EMIT informTypingStatus(mRoomId, true);
            }

            // 3) restart timer.
            mTimer->start();
        }
    } else {
        mTypingInprogress = true;
        Q_EMIT informTypingStatus(roomId, true);
        // Send info about typing.
        // Restart timer.
        mTimer->start();
    }
    mRoomId = roomId;
}

void TypingNotification::slotTimeout()
{
    mTypingInprogress = false;
    // Send info about stopping typing.
    Q_EMIT informTypingStatus(mRoomId, false);
}

#include "moc_typingnotification.cpp"
