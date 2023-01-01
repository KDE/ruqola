/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

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

    void textNotificationChanged(const QString &roomId, bool emptyString);
Q_SIGNALS:
    void informTypingStatus(const QString &room, bool typing);

private:
    Q_DISABLE_COPY(TypingNotification)
    void slotTimeout();
    QString mRoomId;
    bool mTypingInprogress = false;
    QTimer *const mTimer;
};
