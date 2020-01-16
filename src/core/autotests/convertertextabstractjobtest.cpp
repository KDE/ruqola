/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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
