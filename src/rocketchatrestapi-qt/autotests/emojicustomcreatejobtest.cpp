/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojicustomcreatejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "emoji/emojicustomcreatejob.h"
#include "restapimethod.h"
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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(!job.emojiInfo().isValid());
    QVERIFY(!job.hasQueryParameterSupport());
}

void EmojiCustomCreateJobTest::shouldGenerateRequest()
{
    EmojiCustomCreateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/emoji-custom.create"_s));
}

void EmojiCustomCreateJobTest::shouldGenerateJson()
{
    EmojiCustomCreateJob job;
    // TODO
    //    const QString emojiId = u"foo1"_s;
    //    job.setEmojiId(emojiId);
    //    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"emojiId":"%1"})").arg(emojiId).toLatin1());
}

#include "moc_emojicustomcreatejobtest.cpp"
