/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojicustomupdatejobtest.h"
#include "emoji/emojicustomupdatejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(EmojiCustomUpdateJobTest)
using namespace RocketChatRestApi;
EmojiCustomUpdateJobTest::EmojiCustomUpdateJobTest(QObject *parent)
    : QObject(parent)
{
}

void EmojiCustomUpdateJobTest::shouldHaveDefaultValue()
{
    EmojiCustomUpdateJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.emojiInfo().isValid());
    QVERIFY(!job.hasQueryParameterSupport());
}

void EmojiCustomUpdateJobTest::shouldGenerateRequest()
{
    EmojiCustomUpdateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/emoji-custom.update")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void EmojiCustomUpdateJobTest::shouldGenerateJson()
{
    EmojiCustomUpdateJob job;
    // TODO
    //    const QString emojiId = QStringLiteral("foo1");
    //    job.setEmojiId(emojiId);
    //    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"emojiId":"%1"})").arg(emojiId).toLatin1());
}

#include "moc_emojicustomupdatejobtest.cpp"
