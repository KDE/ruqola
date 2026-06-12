/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundscreatejobtest.h"

#include "custom/customsoundscreatejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(CustomSoundsCreateJobTest)
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
CustomSoundsCreateJobTest::CustomSoundsCreateJobTest(QObject *parent)
    : QObject(parent)
{
}

void CustomSoundsCreateJobTest::shouldHaveDefaultValue()
{
    CustomSoundsCreateJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(!job.soundInfo().isValid());
    QVERIFY(!job.hasQueryParameterSupport());
}

void CustomSoundsCreateJobTest::shouldGenerateRequest()
{
    CustomSoundsCreateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/custom-sounds.create"_s));
}

void CustomSoundsCreateJobTest::shouldGenerateJson()
{
    CustomSoundsCreateJob job;
    // TODO
    //    const QString emojiId = u"foo1"_s;
    //    job.setEmojiId(emojiId);
    //    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"emojiId":"%1"})").arg(emojiId).toLatin1());
}

#include "moc_customsoundscreatejobtest.cpp"
