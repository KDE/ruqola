/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfilewidgettest.h"

#include "dialogs/uploadfilewidget.h"
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(UploadFileWidgetTest)

using namespace Qt::Literals::StringLiterals;
UploadFileWidgetTest::UploadFileWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void UploadFileWidgetTest::shouldHaveDefaultValues()
{
    UploadFileWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto layout = w.findChild<QFormLayout *>(u"layout"_s);
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins{});

    auto mFileName = w.findChild<QLineEdit *>(u"mFileName"_s);
    QVERIFY(mFileName);
    QVERIFY(mFileName->isClearButtonEnabled());

    auto mImagePreview = w.findChild<QLabel *>(u"mImagePreview"_s);
    QVERIFY(mImagePreview);
    QVERIFY(!mImagePreview->isVisible());

    auto mFileNameInfo = w.findChild<QLabel *>(u"mFileNameInfo"_s);
    QVERIFY(mFileNameInfo);
    QVERIFY(mFileNameInfo->text().isEmpty());

    auto mMimeTypeLabel = w.findChild<QLabel *>(u"mMimeTypeLabel"_s);
    QVERIFY(mMimeTypeLabel);
    QVERIFY(mMimeTypeLabel->text().isEmpty());
}

void UploadFileWidgetTest::shouldReturnEmptyResult()
{
    UploadFileWidget w;
    QVERIFY(w.fileName().isEmpty());
    QVERIFY(w.fileUrl().isEmpty());
}

#include "moc_uploadfilewidgettest.cpp"
