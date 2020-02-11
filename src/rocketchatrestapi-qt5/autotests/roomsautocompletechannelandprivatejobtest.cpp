/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "roomsautocompletechannelandprivatejobtest.h"
#include "rooms/roomsautocompletechannelandprivatejob.h"
#include "restapimethod.h"
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
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    RoomsAutocompleteChannelAndPrivateJob::RoomsAutocompleteChannelAndPrivateInfo info;
    info.name = QStringLiteral("foo");
    job.setRoomsCompleterInfo(info);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url().toString(), QStringLiteral("http://www.kde.org/api/v1/rooms.autocomplete.channelAndPrivate?selector=%7B%22name%22: %22foo%22%7D"));
}
