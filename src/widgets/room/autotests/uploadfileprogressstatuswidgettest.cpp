/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfileprogressstatuswidgettest.h"
#include "room/uploadfileprogressstatuswidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QTest>
#include <QToolButton>
QTEST_MAIN(UploadFileProgressStatusWidgetTest)
UploadFileProgressStatusWidgetTest::UploadFileProgressStatusWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void UploadFileProgressStatusWidgetTest::shouldHaveDefaultValues()
{
    UploadFileProgressStatusWidget w;

    auto hboxLayout = w.findChild<QHBoxLayout *>(QStringLiteral("hboxLayout"));
    QVERIFY(hboxLayout);
    QCOMPARE(hboxLayout->contentsMargins(), QMargins{});

    auto mProgressBar = w.findChild<QProgressBar *>(QStringLiteral("mProgressBar"));
    QVERIFY(mProgressBar);
    QCOMPARE(mProgressBar->minimum(), 0);
    QCOMPARE(mProgressBar->maximum(), 100);
    QCOMPARE(mProgressBar->value(), -1);

    auto mFileName = w.findChild<QLabel *>(QStringLiteral("mFileName"));
    QVERIFY(mFileName);
    QVERIFY(mFileName->text().isEmpty());

    auto mCancelToolButton = w.findChild<QToolButton *>(QStringLiteral("mCancelToolButton"));
    QVERIFY(mCancelToolButton);
    QVERIFY(!mCancelToolButton->toolTip().isEmpty());

    QCOMPARE(w.identifier(), -1);
}

void UploadFileProgressStatusWidgetTest::shouldChangeText()
{
    UploadFileProgressStatusWidget w;
    const QString text = QStringLiteral("bla");
    auto mFileName = w.findChild<QLabel *>(QStringLiteral("mFileName"));
    QVERIFY(mFileName->text().isEmpty());

    w.setUploadFileName(text);
    QCOMPARE(mFileName->text(), text);
}
