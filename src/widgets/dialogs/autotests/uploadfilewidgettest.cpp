/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfilewidgettest.h"
#include "dialogs/uploadfilewidget.h"
#include <KUrlRequester>
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

    auto mSelectFile = w.findChild<KUrlRequester *>(QStringLiteral("mSelectFile"));
    QVERIFY(mSelectFile);

    auto mImagePreview = w.findChild<QLabel *>(QStringLiteral("mImagePreview"));
    QVERIFY(mImagePreview);
    QVERIFY(!mImagePreview->isVisible());

    auto mFileLabel = w.findChild<QLabel *>(QStringLiteral("mFileLabel"));
    QVERIFY(mFileLabel);
    QVERIFY(!mFileLabel->isVisible());
    QVERIFY(!mFileLabel->text().isEmpty());
}

void UploadFileWidgetTest::shouldReturnEmptyResult()
{
    UploadFileWidget w;
    QVERIFY(w.description().isEmpty());
    QVERIFY(w.fileUrl().isEmpty());
}

#include "moc_uploadfilewidgettest.cpp"
