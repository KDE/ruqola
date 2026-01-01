/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecallwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "conferencecalldialog/conferencecallwidget.h"
#include <QFormLayout>
#include <QTest>
#include <QToolButton>
QTEST_MAIN(ConferenceCallWidgetTest)
ConferenceCallWidgetTest::ConferenceCallWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ConferenceCallWidgetTest::shouldHaveDefaultValues()
{
    ConferenceCallWidget w(nullptr);

    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mMicroButton = w.findChild<QToolButton *>(u"mMicroButton"_s);
    QVERIFY(mMicroButton);
    QVERIFY(!mMicroButton->isChecked());
    QVERIFY(mMicroButton->isCheckable());
    QVERIFY(mMicroButton->autoRaise());

    auto mCameraButton = w.findChild<QToolButton *>(u"mCameraButton"_s);
    QVERIFY(mCameraButton);
    QVERIFY(!mCameraButton->isChecked());
    QVERIFY(mCameraButton->isCheckable());
    QVERIFY(mCameraButton->autoRaise());

    auto info = w.conferenceCallInfo();
    QVERIFY(!info.useCamera);
    QVERIFY(!info.useMic);
}

#include "moc_conferencecallwidgettest.cpp"
