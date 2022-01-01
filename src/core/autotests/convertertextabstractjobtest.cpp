/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "convertertextabstractjobtest.h"
#include "convertertextjob/convertertextabstractjob.h"
#include <QTest>
class ConverterTextAbstractJobImpl : public ConverterTextAbstractJob
{
public:
    explicit ConverterTextAbstractJobImpl(QObject *parent = nullptr)
        : ConverterTextAbstractJob(parent)
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
}
