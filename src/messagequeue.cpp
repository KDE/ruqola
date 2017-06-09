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

#include "ddpclient.h"
#include "ruqola.h"

QQueue<QPair<int,QJsonDocument>> MessageQueue::messageQueue() {
    return m_messageQueue;
}

QHash<int,bool> MessageQueue::messageStatus() {
    return m_messageStatus;
}

MessageQueue::MessageQueue()
{
    connect(Ruqola::self()->ddp(), &DDPClient::loginStatusChanged, this, &MessageQueue::onLoginStatusChanged);
    qDebug() << "Creating message Model";
    QDir cacheDir(Ruqola::self()->cacheBasePath());

    // load unsent messages cache
    if (QFile::exists(cacheDir.absoluteFilePath("UnsentMessagesCache"))) {
        QFile f(cacheDir.absoluteFilePath("UnsentMessagesCache"));
        if (f.open(QIODevice::ReadOnly)) {
            QDataStream in(&f);
            while (!f.atEnd()) {
                char * byteArray;
                quint32 length;
                in.readBytes(byteArray, length);
                QByteArray ba = QByteArray::fromRawData(byteArray, length);
                QByteArray mid;
                mid.append(ba.at(ba.size()-1)).append(ba.at(ba.size()));
                int m_uid = mid.toInt(NULL,10);
                QJsonDocument params = QJsonDocument::fromBinaryData(ba);
                m_messageQueue.enqueue(qMakePair(m_uid,params));
            }
        }
    }
}

void MessageQueue::onLoginStatusChanged()
{
    if (Ruqola::self()->ddp()->loginStatus() == DDPClient::LoggedIn && !m_messageQueue.empty()){
        //retry sending messages
        retry();
    } else if (Ruqola::self()->ddp()->loginStatus() != DDPClient::LoggedIn && !m_messageQueue.empty()) {
        //save messages in messageQueue in local cache and retry after client is loggedIn

        QDir cacheDir(Ruqola::self()->cacheBasePath());
        qDebug() << "Caching Unsent messages to..." << cacheDir.path();
        if (!cacheDir.exists(cacheDir.path())) {
            cacheDir.mkpath(cacheDir.path());
        }
        QFile f(cacheDir.absoluteFilePath("UnsentMessagesCache"));
        if (f.open(QIODevice::WriteOnly)) {
            QDataStream out(&f);
            QQueue<QPair<int,QJsonDocument>>::iterator it;
            for ( it = m_messageQueue.begin(); it != m_messageQueue.end(); it++ ) {
                QPair<int,QJsonDocument> pair = *it;
                QByteArray ba;
                ba = pair.second.toBinaryData();
                if ( pair.first < 10 ) { //so that we can take last 2 digits as m_uid
                    ba.append(QString("0"+pair.first).toInt(NULL,10));
                } else {
                    ba.append(QString(pair.first).toInt(NULL,10));
                }
                out.writeBytes(ba, ba.size());
            }
        }
    }
}


void MessageQueue::retry()
{
    if (  Ruqola::self()->loginStatus() == DDPClient::LoggedIn && !m_messageQueue.empty() ){
        while ( Ruqola::self()->loginStatus() == DDPClient::LoggedIn && !m_messageQueue.empty() ){
            QPair<int,QJsonDocument> pair = m_messageQueue.head();
            int id = pair.first;
            QJsonDocument params = pair.second;
            Ruqola::self()->ddp()->method("sendMessage", params);

            //if it is sent successfully, dequeue it
            //else it'll stay at head in queue for sending again
            QHash<int,bool>::iterator it = m_messageStatus.find(id);
            if ( it!= m_messageStatus.end() && it.value() == true ){
                m_messageQueue.dequeue();
            }
        }
    }
}
