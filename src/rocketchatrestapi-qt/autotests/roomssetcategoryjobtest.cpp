/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomssetcategoryjobtest.h"

#include "rooms/roomssetcategoryjob.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(RoomsSetCategoryJobTest)
using namespace RocketChatRestApi;
RoomsSetCategoryJobTest::RoomsSetCategoryJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomsSetCategoryJobTest::shouldHaveDefaultValue()
{
    RoomsSetCategoryJob job;
    QVERIFY(job.categoryId().isEmpty());
    QVERIFY(job.roomIds().isEmpty());
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void RoomsSetCategoryJobTest::shouldGenerateRequest()
{
    {
        RoomsSetCategoryJob job;
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/experimental/rooms.setCategory"_s));
        QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
    }
}

void RoomsSetCategoryJobTest::shouldGenerateJson()
{
    {
        RoomsSetCategoryJob job;

        const QByteArray categoryId("rr1"_ba);
        job.setCategoryId(categoryId);
        const QList<QByteArray> roomIds({"foo1", "bla"});
        job.setRoomIds(roomIds);

        QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"category":"%1","roomIds":["foo1","bla"]})"_s.arg(QString::fromLatin1(categoryId)).toLatin1());
    }
}

void RoomsSetCategoryJobTest::shouldNotStarting()
{
    RoomsSetCategoryJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);

    const QByteArray categoryId("rr1"_ba);
    job.setCategoryId(categoryId);
    QVERIFY(!job.canStart());
    const QList<QByteArray> roomIds({"foo1", "bla"});
    job.setRoomIds(roomIds);
    QVERIFY(job.canStart());
}

#include "moc_roomssetcategoryjobtest.cpp"
