/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

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
    QVERIFY(!info.defaultSound());
    // TODO
}

void CustomSoundInfoTest::shouldGenerateUrl()
{
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(QStringLiteral("jjj"));
        info.setName(QStringLiteral("bla"));
        info.setIdentifier(QByteArrayLiteral("foo"));
        QCOMPARE(info.generateUrl(), QStringLiteral("/sounds/foo.jjj"));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(false);
        info.setExtension(QStringLiteral("bbb"));
        info.setName(QStringLiteral("bla"));
        info.setIdentifier(QByteArrayLiteral("kde"));
        QCOMPARE(info.generateUrl(), QStringLiteral("/custom-sounds/kde.bbb"));
    }
}

#include "moc_customsoundinfotest.cpp"
