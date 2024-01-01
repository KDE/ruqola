/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "convertertextabstractjobtest.h"
#include "convertertextjob/convertertextabstractjob.h"
#include <QTest>
class ConverterTextAbstractJobImpl : public ConverterTextAbstractJob
{
public:
    explicit ConverterTextAbstractJobImpl(QObject *parent = nullptr)
        : ConverterTextAbstractJob(nullptr, parent)
    {
    }

    void start() override
    {
    }
};

QTEST_GUILESS_MAIN(ConverterTextAbstractJobTest)

ConverterTextAbstractJobTest::ConverterTextAbstractJobTest(QObject *parent)
    : QObject(parent)
{
}

void ConverterTextAbstractJobTest::shouldHaveDefaultValue()
{
    ConverterTextAbstractJobImpl job;
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(!job.rocketChatAcount());
}

#include "moc_convertertextabstractjobtest.cpp"
