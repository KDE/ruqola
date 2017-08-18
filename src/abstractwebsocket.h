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

#ifndef ABSTRACTWEBSOCKET_H
#define ABSTRACTWEBSOCKET_H

#include <QObject>

class AbstractWebSocket : public QObject
{
    Q_OBJECT
public:
    explicit AbstractWebSocket(QObject *parent = nullptr);
    ~AbstractWebSocket();

    virtual void openUrl(const QUrl &url) = 0;
    virtual qint64 sendTextMessage(const QString &message) = 0;
    virtual bool isValid() const = 0;
    virtual void flush() = 0;
    virtual void close() = 0;

Q_SIGNALS:
    void disconnected();
    void connected();
    void textMessageReceived(const QString &message);

};

#endif // ABSTRACTWEBSOCKET_H
