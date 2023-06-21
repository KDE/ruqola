/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagecachetest.h"
#include "chat/getmessagejob.h"
#include "messagecache.h"
#include "messages/message.h"

#include <QJsonDocument>
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
        if (auto getMsgJob = qobject_cast<RocketChatRestApi::GetMessageJob *>(job)) {
            QTimer::singleShot(100, this, [getMsgJob]() {
                const QByteArray content =
                    "{ \"message\": "
                    "{ \"msg\": \"message foo\","
                    "  \"_id\": \"Co6LnNbu5TYcXPuMG\" } }";
                const QJsonDocument doc = QJsonDocument::fromJson(content);
                Q_EMIT getMsgJob->getMessageDone(doc.object(), getMsgJob->messageId(), getMsgJob->roomId());
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
    MyMessageCache cache(nullptr);
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

#include "moc_messagecachetest.cpp"
