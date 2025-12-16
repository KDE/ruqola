/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojicustomdeletejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "emoji/emojicustomdeletejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.emojiId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void EmojiCustomDeleteJobTest::shouldGenerateRequest()
{
    EmojiCustomDeleteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/emoji-custom.delete"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void EmojiCustomDeleteJobTest::shouldGenerateJson()
{
    EmojiCustomDeleteJob job;
    const QByteArray emojiId("foo1");
    job.setEmojiId(emojiId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"emojiId":"%1"})").arg(QLatin1StringView(emojiId)).toLatin1());
}

#include "moc_emojicustomdeletejobtest.cpp"
