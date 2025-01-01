/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

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

UploadFileWidgetTest::UploadFileWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void UploadFileWidgetTest::shouldHaveDefaultValues()
{
    UploadFileWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto layout = w.findChild<QFormLayout *>(QStringLiteral("layout"));
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins{});

    auto mDescription = w.findChild<QLineEdit *>(QStringLiteral("mDescription"));
    QVERIFY(mDescription);
    QVERIFY(mDescription->isClearButtonEnabled());

    auto mFileName = w.findChild<QLineEdit *>(QStringLiteral("mFileName"));
    QVERIFY(mFileName);
    QVERIFY(mFileName->isClearButtonEnabled());

    auto mImagePreview = w.findChild<QLabel *>(QStringLiteral("mImagePreview"));
    QVERIFY(mImagePreview);
    QVERIFY(!mImagePreview->isVisible());

    auto mFileNameInfo = w.findChild<QLabel *>(QStringLiteral("mFileNameInfo"));
    QVERIFY(mFileNameInfo);
    QVERIFY(mFileNameInfo->text().isEmpty());

    auto mMimeTypeLabel = w.findChild<QLabel *>(QStringLiteral("mMimeTypeLabel"));
    QVERIFY(mMimeTypeLabel);
    QVERIFY(mMimeTypeLabel->text().isEmpty());
}

void UploadFileWidgetTest::shouldReturnEmptyResult()
{
    UploadFileWidget w;
    QVERIFY(w.description().isEmpty());
    QVERIFY(w.fileUrl().isEmpty());
}

#include "moc_uploadfilewidgettest.cpp"
