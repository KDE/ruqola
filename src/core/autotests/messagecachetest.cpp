/*
   Copyright (c) 2020 David Faure <faure@kde.org>

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

#include "messagecachetest.h"
#include "messagecache.h"
#include "chat/getmessagejob.h"
#include "messages/message.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>
#include <QTimer>

QTEST_GUILESS_MAIN(MessageCacheTest)

class MyMessageCache : public MessageCache
{
public:
    using MessageCache::MessageCache;
    int jobsStarted() const
    {
        return mJobStarted;
    }

protected:
    bool startJob(RocketChatRestApi::RestApiAbstractJob *job) override
    {
        if (auto *getMsgJob = qobject_cast<RocketChatRestApi::GetMessageJob *>(job)) {
            QTimer::singleShot(100, this, [getMsgJob]() {
                const QByteArray content = "{ \"message\": "
                                           "{ \"msg\": \"message foo\","
                                           "  \"_id\": \"Co6LnNbu5TYcXPuMG\" } }";
                const QJsonDocument doc = QJsonDocument::fromJson(content);
                Q_EMIT getMsgJob->getMessageDone(doc.object(), getMsgJob->messageId());
            });
            ++mJobStarted;
            return true;
        }
        return false;
    }

private:
    int mJobStarted = 0;
};

MessageCacheTest::MessageCacheTest(QObject *parent)
    : QObject(parent)
{
}

void MessageCacheTest::shouldLoadSingleMessage()
{
    // GIVEN
    MyMessageCache cache;
    const QString messageId = QStringLiteral("Co6LnNbu5TYcXPuMG");
    QSignalSpy loadedSpy(&cache, &MessageCache::messageLoaded);

    // WHEN
    Message *msg = cache.messageForId(messageId);
    QVERIFY(!msg);
    msg = cache.messageForId(messageId);
    QVERIFY(!msg);

    // THEN
    QCOMPARE(cache.jobsStarted(), 1);
    QVERIFY(loadedSpy.isEmpty());
    QVERIFY(loadedSpy.wait());
    msg = cache.messageForId(messageId);
    QVERIFY(msg);
    QCOMPARE(msg->messageId(), messageId);
    QCOMPARE(cache.jobsStarted(), 1);
}
