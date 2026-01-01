/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createsoundmessagewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/createsoundmessagewidget.h"
#include <QComboBox>
#include <QLabel>
#include <QTest>
#include <QToolButton>
#include <QVBoxLayout>

QTEST_MAIN(CreateSoundMessageWidgetTest)
CreateSoundMessageWidgetTest::CreateSoundMessageWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void CreateSoundMessageWidgetTest::shouldHaveDefaultValues()
{
#if 0
    CreateSoundMessageWidget *w = new CreateSoundMessageWidget(nullptr);

    auto mainLayout = w->findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mDeviceComboBox = w->findChild<QComboBox *>(u"mDeviceComboBox"_s);
    QVERIFY(mDeviceComboBox);

    auto mRecordButton = w->findChild<QToolButton *>(u"mRecordButton"_s);
    QVERIFY(mRecordButton);

    auto mPauseButton = w->findChild<QToolButton *>(u"mPauseButton"_s);
    QVERIFY(mPauseButton);

    auto mStopButton = w->findChild<QToolButton *>(u"mStopButton"_s);
    QVERIFY(mStopButton);

    auto mLabelDuration = w->findChild<QLabel *>(u"mLabelDuration"_s);
    QVERIFY(mLabelDuration);
    QCOMPARE(mLabelDuration->textFormat(), Qt::PlainText);
    w->deleteLater();
#endif
}

#include "moc_createsoundmessagewidgettest.cpp"
