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
#include <QObject>


class MessageQueue : public QObject
{
    Q_OBJECT
public:

    explicit MessageQueue(QObject *parent = nullptr);
    ~MessageQueue();

    /**
    * @brief Retry to send unsent messages in DDPClient's abtract message queue
    */
    void processQueue();

    /**
    * @brief Constructs QPair<QString,QJsonDocument> object from QJsonObject
    *
    * @param object The Json containing message attributes
    * @return QPair<QString,QJsonDocument>, The pair containing the method and params
    */
    static QPair<QString,QJsonDocument> fromJson(const QJsonObject &object);

    /**
    * @brief Constructs QBytearray from QPair<QString,QJsonDocument> object
    *
    * @param pair The pair containing method and params
    * @return QByteArray, The Json containing message attributes
    */
    static QByteArray serialize(const QPair<QString, QJsonDocument> &pair);

public slots:
    void onLoginStatusChanged();

};


#endif // MESSAGEQUEUE_H
