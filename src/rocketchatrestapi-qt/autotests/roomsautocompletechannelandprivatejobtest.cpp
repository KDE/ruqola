/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsautocompletechannelandprivatejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rooms/roomsautocompletechannelandprivatejob.h"
#include <QTest>
QTEST_GUILESS_MAIN(RoomsAutocompleteChannelAndPrivateJobTest)
using namespace RocketChatRestApi;
RoomsAutocompleteChannelAndPrivateJobTest::RoomsAutocompleteChannelAndPrivateJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomsAutocompleteChannelAndPrivateJobTest::shouldHaveDefaultValue()
{
    RoomsAutocompleteChannelAndPrivateJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.roomsCompleterInfo().isValid());
}

void RoomsAutocompleteChannelAndPrivateJobTest::shouldGenerateRequest()
{
    RoomsAutocompleteChannelAndPrivateJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    RoomsAutocompleteChannelAndPrivateJob::RoomsAutocompleteChannelAndPrivateInfo info;
    info.name = u"foo"_s;
    job.setRoomsCompleterInfo(info);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url().toString(), u"http://www.kde.org/api/v1/rooms.autocomplete.channelAndPrivate?selector=%7B%22name%22: %22foo%22%7D"_s);
}

#include "moc_roomsautocompletechannelandprivatejobtest.cpp"
