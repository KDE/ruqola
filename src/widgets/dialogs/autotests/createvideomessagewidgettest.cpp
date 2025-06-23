/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createvideomessagewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/createvideomessagewidget.h"
#include <QTest>
#include <QToolButton>
#include <QVBoxLayout>
QTEST_MAIN(CreateVideoMessageWidgetTest)

CreateVideoMessageWidgetTest::CreateVideoMessageWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void CreateVideoMessageWidgetTest::shouldHaveDefaultValues()
{
    CreateVideoMessageWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mStopButton = w.findChild<QToolButton *>(u"mStopButton"_s);
    QVERIFY(mStopButton);

    auto mPauseButton = w.findChild<QToolButton *>(u"mStopButton"_s);
    QVERIFY(mPauseButton);

    auto mRecordButton = w.findChild<QToolButton *>(u"mRecordButton"_s);
    QVERIFY(mRecordButton);
}

#include "moc_createvideomessagewidgettest.cpp"
