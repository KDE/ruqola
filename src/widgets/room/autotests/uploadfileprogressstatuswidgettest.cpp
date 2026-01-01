/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfileprogressstatuswidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto hboxLayout = w.findChild<QHBoxLayout *>(u"hboxLayout"_s);
    QVERIFY(hboxLayout);
    QCOMPARE(hboxLayout->contentsMargins(), QMargins{});

    auto mProgressBar = w.findChild<QProgressBar *>(u"mProgressBar"_s);
    QVERIFY(mProgressBar);
    QCOMPARE(mProgressBar->minimum(), 0);
    QCOMPARE(mProgressBar->maximum(), 100);
    QCOMPARE(mProgressBar->value(), -1);

    auto mFileName = w.findChild<QLabel *>(u"mFileName"_s);
    QVERIFY(mFileName);
    QVERIFY(mFileName->text().isEmpty());

    auto mCancelToolButton = w.findChild<QToolButton *>(u"mCancelToolButton"_s);
    QVERIFY(mCancelToolButton);
    QVERIFY(!mCancelToolButton->toolTip().isEmpty());

    QCOMPARE(w.identifier(), -1);
}

void UploadFileProgressStatusWidgetTest::shouldChangeText()
{
    UploadFileProgressStatusWidget w;
    const QString text = u"bla"_s;
    auto mFileName = w.findChild<QLabel *>(u"mFileName"_s);
    QVERIFY(mFileName->text().isEmpty());

    w.setUploadFileName(text);
    QCOMPARE(mFileName->text(), text);
}

#include "moc_uploadfileprogressstatuswidgettest.cpp"
