/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "emojicustomcreatejobtest.h"
#include "emoji/emojicustomcreatejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(EmojiCustomCreateJobTest)
using namespace RocketChatRestApi;
EmojiCustomCreateJobTest::EmojiCustomCreateJobTest(QObject *parent)
    : QObject(parent)
{
}

void EmojiCustomCreateJobTest::shouldHaveDefaultValue()
{
    EmojiCustomCreateJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.emojiInfo().isValid());
    QVERIFY(!job.hasQueryParameterSupport());
}

void EmojiCustomCreateJobTest::shouldGenerateRequest()
{
    EmojiCustomCreateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/emoji-custom.create")));
}

void EmojiCustomCreateJobTest::shouldGenerateJson()
{
    EmojiCustomCreateJob job;
    // TODO
    //    const QString emojiId = QStringLiteral("foo1");
    //    job.setEmojiId(emojiId);
    //    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"emojiId":"%1"})").arg(emojiId).toLatin1());
}
