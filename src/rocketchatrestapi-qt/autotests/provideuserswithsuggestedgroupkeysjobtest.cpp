/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "provideuserswithsuggestedgroupkeysjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "e2e/provideuserswithsuggestedgroupkeysjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(ProvideUsersWithSuggestedGroupKeysJobTest)
using namespace RocketChatRestApi;
ProvideUsersWithSuggestedGroupKeysJobTest::ProvideUsersWithSuggestedGroupKeysJobTest(QObject *parent)
    : QObject(parent)
{
}

void ProvideUsersWithSuggestedGroupKeysJobTest::shouldHaveDefaultValue()
{
    ProvideUsersWithSuggestedGroupKeysJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.keys().isEmpty());
}

void ProvideUsersWithSuggestedGroupKeysJobTest::shouldGenerateRequest()
{
    ProvideUsersWithSuggestedGroupKeysJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/e2e.provideUsersSuggestedGroupKeys"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ProvideUsersWithSuggestedGroupKeysJobTest::shouldGenerateJson()
{
    ProvideUsersWithSuggestedGroupKeysJob job;
    job.setRoomId(u"roomid"_s);

    // A member which only needs the current key: the endpoint refuses unknown members, so no empty
    // "oldKeys" must appear.
    {
        const SuggestedGroupKey key{
            .userId = u"user1"_s,
            .encryptedKey = u"kid1payload"_s,
            .oldKeys = {},
        };
        job.setKeys({key});
        QCOMPARE(job.json().toJson(QJsonDocument::Compact), R"({"usersSuggestedGroupKeys":{"roomid":[{"_id":"user1","key":"kid1payload"}]}})"_ba);
    }

    // A member which also gets the keys of the room's earlier eras, without which it can read
    // nothing written before the last key change.
    {
        const SuggestedGroupKey key{
            .userId = u"user1"_s,
            .encryptedKey = u"kid1payload"_s,
            .oldKeys =
                {
                    {
                        .keyId = u"kid0"_s,
                        .encryptedKey = u"kid0payload"_s,
                        .timeStamp = u"2026-08-27T09:00:00.000Z"_s,
                    },
                },
        };
        job.setKeys({key});
        QCOMPARE(job.json().toJson(QJsonDocument::Compact),
                 R"({"usersSuggestedGroupKeys":{"roomid":[{"_id":"user1","key":"kid1payload","oldKeys":[{"E2EKey":"kid0payload",)"
                 R"("e2eKeyId":"kid0","ts":"2026-08-27T09:00:00.000Z"}]}]}})"_ba);
    }
}

#include "moc_provideuserswithsuggestedgroupkeysjobtest.cpp"
