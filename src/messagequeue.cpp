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

#include "ruqola.h"
#include "ddpclient.h"
#include "messagemodel.h"
#include <QMessageBox>
#include <QtNetwork>


MessageQueue::MessageQueue()
{
    connect(Ruqola::self()->ddp(), &DDPClient::loginStatusChanged, this, &MessageQueue::onLoginStatusChanged);
    QDir cacheDir(Ruqola::self()->ddp()->cachePath());

    // load unsent messages cache
    if (QFile::exists(cacheDir.absoluteFilePath("QueueCache"))) {
        QFile f(cacheDir.absoluteFilePath("QueueCache"));
        if (f.open(QIODevice::ReadOnly)) {
            QDataStream in(&f);
            while (!f.atEnd()) {
                char * byteArray;
                quint32 length;
                in.readBytes(byteArray, length);
                QByteArray ba = QByteArray::fromRawData(byteArray, length);
                //Find out what to store and retrieve
                QByteArray method;
                QString message = QString(ba);
                Ruqola::self()->ddp()->messageQueue().enqueue(qMakePair(method,message));
            }
        }
    }
}

void cacheQueue()
{
    QDir cacheDir(Ruqola::self()->ddp()->cachePath());
    qDebug() << "Caching Unsent messages to..." << cacheDir.path();
    if (!cacheDir.exists(cacheDir.path())) {
        cacheDir.mkpath(cacheDir.path());
    }
    QFile f(cacheDir.absoluteFilePath("QueueCache"));
    if (f.open(QIODevice::WriteOnly)) {
        QDataStream out(&f);
        QQueue<QPair<QString,QString>>::iterator it;
        //Find out what to store and retrieve
        QQueue<QPair<QString,QString>> queue = Ruqola::self()->ddp()->messageQueue();
        for ( it = queue.begin(); it != queue.end(); it++ ) {
            QPair<QString,QString> pair = *it;
            QByteArray ba;
            ba.append(pair.first.toLatin1());
            ba.append(pair.second.toLatin1());
            out.writeBytes(ba, ba.size());
        }
    }
}


MessageQueue::~MessageQueue()
{
    cacheQueue();
}


bool internetConnection()
{
    QNetworkAccessManager nam;
    QNetworkRequest req(QUrl("http://www.google.com"));
    QNetworkReply *reply = nam.get(req);
    QEventLoop loop;
//    connect(&reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    if(reply->bytesAvailable()){
        return true;
    } else {
        return false;
    }
}

void MessageQueue::onLoginStatusChanged()
{
    if (Ruqola::self()->loginStatus() == DDPClient::LoggedIn
             && !Ruqola::self()->ddp()->messageQueue().empty()){

        processQueue(); //retry sending messages

    } else if (Ruqola::self()->loginStatus() == DDPClient::LoggedIn
          && !internetConnection() && !Ruqola::self()->ddp()->messageQueue().empty()) {

        QMessageBox msgBox;
        msgBox.setText("You are not connected to the internet");
        msgBox.exec();

        cacheQueue();

    } else if (Ruqola::self()->loginStatus() != DDPClient::LoggedIn
               && !Ruqola::self()->ddp()->messageQueue().empty()) {

        //save messages in messageQueue in local cache and retry after client is loggedIn
        cacheQueue();
    }
}


void MessageQueue::processQueue()
{
    while ( Ruqola::self()->loginStatus() == DDPClient::LoggedIn
             && !Ruqola::self()->ddp()->messageQueue().empty() ){

        //If in between client goes offline, break
            if(!internetConnection()){
                break;
            }

            //Find out what to store and retrieve
            QPair<QString,QString> pair = Ruqola::self()->ddp()->messageQueue().head();
            QString method = pair.first;
            QString message = pair.second;
            QJsonObject params;
//            Ruqola::self()->ddp()->method(method, params);
    }
}
