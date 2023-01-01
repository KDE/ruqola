/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecallwidgettest.h"
#include "conferencecalldialog/conferencecallwidget.h"
#include <QTest>
#include <QToolButton>
#include <QVBoxLayout>
QTEST_MAIN(ConferenceCallWidgetTest)
ConferenceCallWidgetTest::ConferenceCallWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ConferenceCallWidgetTest::shouldHaveDefaultValues()
{
    ConferenceCallWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mMicroButton = w.findChild<QToolButton *>(QStringLiteral("mMicroButton"));
    QVERIFY(mMicroButton);
    QVERIFY(!mMicroButton->isChecked());
    QVERIFY(mMicroButton->isCheckable());
    QVERIFY(mMicroButton->autoRaise());

    auto mCameraButton = w.findChild<QToolButton *>(QStringLiteral("mCameraButton"));
    QVERIFY(mCameraButton);
    QVERIFY(!mCameraButton->isChecked());
    QVERIFY(mCameraButton->isCheckable());
    QVERIFY(mCameraButton->autoRaise());

    auto info = w.startInfo();
    QVERIFY(!info.useCamera);
    QVERIFY(!info.useMic);
}
