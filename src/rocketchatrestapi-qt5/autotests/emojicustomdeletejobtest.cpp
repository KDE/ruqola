/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojicustomdeletejobtest.h"
#include "emoji/emojicustomdeletejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(EmojiCustomDeleteJobTest)
using namespace RocketChatRestApi;
EmojiCustomDeleteJobTest::EmojiCustomDeleteJobTest(QObject *parent)
    : QObject(parent)
{
}

void EmojiCustomDeleteJobTest::shouldHaveDefaultValue()
{
    EmojiCustomDeleteJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.emojiId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void EmojiCustomDeleteJobTest::shouldGenerateRequest()
{
    EmojiCustomDeleteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/emoji-custom.delete")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void EmojiCustomDeleteJobTest::shouldGenerateJson()
{
    EmojiCustomDeleteJob job;
    const QString emojiId = QStringLiteral("foo1");
    job.setEmojiId(emojiId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"emojiId":"%1"})").arg(emojiId).toLatin1());
}
