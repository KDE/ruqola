/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <QtCore>

class MessageQueue : public QObject
{
    Q_OBJECT
public:

    // method which tries to resend unsuccessful messages again
    void retry();

    MessageQueue();

    QQueue<QPair<int,QJsonDocument>> messageQueue();
    QHash<int,bool> messageStatus();

public slots:
    void onLoginStatusChanged();

private:
    //pair- int (m_uid), QJsonDocument (params)
    QQueue<QPair<int,QJsonDocument>> m_messageQueue;

    //message with m_uid sent succussfully or not
    QHash<int,bool> m_messageStatus;

};


#endif // MESSAGEQUEUE_H
