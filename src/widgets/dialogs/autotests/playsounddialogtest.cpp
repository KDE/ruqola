/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "playsounddialogtest.h"
#include "dialogs/playsounddialog.h"
#include "dialogs/playsoundwidget.h"
#include <QComboBox>
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(PlaySoundDialogTest)
PlaySoundDialogTest::PlaySoundDialogTest(QObject *parent)
    : QObject(parent)
{
}

void PlaySoundDialogTest::shouldHaveDefaultValues()
{
    PlaySoundDialog w;

    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mSoundWidget = w.findChild<PlaySoundWidget *>(QStringLiteral("mSoundWidget"));
    QVERIFY(mSoundWidget);

    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);

    auto mDeviceComboBox = w.findChild<QComboBox *>(QStringLiteral("mDeviceComboBox"));
    QVERIFY(mDeviceComboBox);
}
