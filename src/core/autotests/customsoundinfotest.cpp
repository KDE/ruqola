/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundinfotest.h"
#include "customsound/customsoundinfo.h"
#include <QTest>
using namespace Qt::Literals::StringLiterals;
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
    QVERIFY(!info.defaultSound());
    // TODO
}

void CustomSoundInfoTest::shouldGenerateUrl()
{
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(u"jjj"_s);
        info.setName(u"bla"_s);
        info.setIdentifier("foo"_ba);
        QCOMPARE(info.generateUrl(), u"/sounds/foo.jjj"_s);
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(false);
        info.setExtension(u"bbb"_s);
        info.setName(u"bla"_s);
        info.setIdentifier("kde"_ba);
        QCOMPARE(info.generateUrl(), u"/custom-sounds/kde.bbb"_s);
    }
}

#include "moc_customsoundinfotest.cpp"
