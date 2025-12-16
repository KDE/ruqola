/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojicustomupdatejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "emoji/emojicustomupdatejob.h"
#include "restapimethod.h"
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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(!job.emojiInfo().isValid());
    QVERIFY(!job.hasQueryParameterSupport());
}

void EmojiCustomUpdateJobTest::shouldGenerateRequest()
{
    EmojiCustomUpdateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/emoji-custom.update"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void EmojiCustomUpdateJobTest::shouldGenerateJson()
{
    EmojiCustomUpdateJob job;
    // TODO
    //    const QString emojiId = u"foo1"_s;
    //    job.setEmojiId(emojiId);
    //    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"emojiId":"%1"})").arg(emojiId).toLatin1());
}

#include "moc_emojicustomupdatejobtest.cpp"
