/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "playsounddialogtest.h"
using namespace Qt::Literals::StringLiterals;

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
    PlaySoundDialog w(nullptr);

    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mSoundWidget = w.findChild<PlaySoundWidget *>(u"mSoundWidget"_s);
    QVERIFY(mSoundWidget);

    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    auto mDeviceComboBox = w.findChild<QComboBox *>(u"mDeviceComboBox"_s);
    QVERIFY(mDeviceComboBox);
}

#include "moc_playsounddialogtest.cpp"
