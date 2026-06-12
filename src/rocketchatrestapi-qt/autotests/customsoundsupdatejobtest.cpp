/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundsupdatejobtest.h"

#include "custom/customsoundsupdatejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(CustomSoundsUpdateJobTest)
using namespace RocketChatRestApi;
CustomSoundsUpdateJobTest::CustomSoundsUpdateJobTest(QObject *parent)
    : QObject(parent)
{
}

void CustomSoundsUpdateJobTest::shouldHaveDefaultValue()
{
    CustomSoundsUpdateJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(!job.soundInfo().isValid());
    QVERIFY(!job.hasQueryParameterSupport());
}

void CustomSoundsUpdateJobTest::shouldGenerateRequest()
{
    CustomSoundsUpdateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/custom-sounds.update"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void CustomSoundsUpdateJobTest::shouldGenerateJson()
{
    CustomSoundsUpdateJob job;
    // TODO
    //    const QString emojiId = u"foo1"_s;
    //    job.setEmojiId(emojiId);
    //    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"emojiId":"%1"})").arg(emojiId).toLatin1());
}

#include "moc_customsoundsupdatejobtest.cpp"
