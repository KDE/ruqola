/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "downloadfilejobtest.h"
#include "downloadfilejob.h"
#include <QTest>
QTEST_GUILESS_MAIN(DownloadFileJobTest)
using namespace RocketChatRestApi;
DownloadFileJobTest::DownloadFileJobTest(QObject *parent)
    : QObject(parent)
{
}

void DownloadFileJobTest::shouldHaveDefaultValue()
{
    // TODO
    // QVERIFY(!job.hasQueryParameterSupport());
}

#include "moc_downloadfilejobtest.cpp"
