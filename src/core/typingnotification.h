/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QObject>
class QTimer;
class LIBRUQOLACORE_TESTS_EXPORT TypingNotification : public QObject
{
    Q_OBJECT
public:
    explicit TypingNotification(QObject *parent = nullptr);
    ~TypingNotification() override;

    void textNotificationChanged(const QByteArray &roomId, bool emptyString);
Q_SIGNALS:
    void informTypingStatus(const QByteArray &room, bool typing);

private:
    void slotTimeout();
    QByteArray mRoomId;
    bool mTypingInprogress = false;
    QTimer *const mTimer;
};
