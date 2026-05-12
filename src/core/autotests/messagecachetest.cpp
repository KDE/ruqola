/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagecachetest.h"

#include "messagecache.h"
#include "messages/message.h"
#include "misc/methodcalljob.h"

#include "rocketchatmessage.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QSignalSpy>
#include <QTest>
#include <QTimer>

using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(MessageCacheTest)

class MyMessageCache : public MessageCache
{
public:
    using MessageCache::MessageCache;
    [[nodiscard]] int jobsStarted() const
    {
        return mJobStarted;
    }

protected:
    [[nodiscard]] RocketChatRestApi::MethodCallJob::MethodCallJobInfo generateMethodCallInfo(const QByteArray &messageId) override
    {
        RocketChatRestApi::MethodCallJob::MethodCallJobInfo info;
        info.methodName = u"getSingleMessage"_s;
        const QJsonArray params{QString::fromLatin1(messageId)};
        info.messageObj = RocketChatMessage::generateJsonObject(info.methodName, params, 3);
        info.anonymous = false;
        return info;
    }

    bool startJob(RocketChatRestApi::RestApiAbstractJob *job) override
    {
        if (auto getMsgJob = qobject_cast<RocketChatRestApi::MethodCallJob *>(job)) {
            QTimer::singleShot(100, this, [getMsgJob]() {
                const QByteArray content =
                    "{ \"result\":"
                    "{ \"msg\": \"message foo\","
                    "  \"_id\": \"Co6LnNbu5TYcXPuMG\" } }";
                const QJsonDocument doc = QJsonDocument::fromJson(content);
                Q_EMIT getMsgJob->methodCallDone(doc.object());
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
    const QByteArray messageId = "Co6LnNbu5TYcXPuMG"_ba;
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
