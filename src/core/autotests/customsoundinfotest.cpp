/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundinfotest.h"
#include "customsound/customsoundinfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(CustomSoundInfoTest)
CustomSoundInfoTest::CustomSoundInfoTest(QObject *parent)
    : QObject(parent)
{
}

void CustomSoundInfoTest::shouldHaveDefaultValues()
{
    CustomSoundInfo info;
    QVERIFY(info.identifier().isEmpty());
    QVERIFY(info.name().isEmpty());
    QVERIFY(info.extension().isEmpty());
    QVERIFY(!info.isValid());
    // TODO
}

#include "moc_customsoundinfotest.cpp"
