/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundsgetonejobtest.h"

#include "custom/customsoundsgetonejob.h"
#include "restapimethod.h"
#include <QTest>

QTEST_GUILESS_MAIN(CustomSoundsGetOneJobTest)
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
CustomSoundsGetOneJobTest::CustomSoundsGetOneJobTest(QObject *parent)
    : QObject(parent)
{
}

void CustomSoundsGetOneJobTest::shouldHaveDefaultValue()
{
    CustomSoundsGetOneJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.soundId().isEmpty());
}

void CustomSoundsGetOneJobTest::shouldGenerateRequest()
{
    CustomSoundsGetOneJob job;
    const QByteArray soundId{"foo"_ba};
    job.setSoundId(soundId);
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/custom-sounds.getOne?_id=%1"_s.arg(QString::fromLatin1(soundId))));
}

#include "moc_customsoundsgetonejobtest.cpp"
