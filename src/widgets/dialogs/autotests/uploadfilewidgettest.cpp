/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfilewidgettest.h"
#include "dialogs/uploadfilewidget.h"
#include <KUrlRequester>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(UploadFileWidgetTest)

UploadFileWidgetTest::UploadFileWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void UploadFileWidgetTest::shouldHaveDefaultValues()
{
    UploadFileWidget w;
    auto layout = w.findChild<QFormLayout *>(QStringLiteral("layout"));
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins{});

    auto mDescription = w.findChild<QLineEdit *>(QStringLiteral("mDescription"));
    QVERIFY(mDescription);
    QVERIFY(mDescription->isClearButtonEnabled());

    auto mSelectFile = w.findChild<KUrlRequester *>(QStringLiteral("mSelectFile"));
    QVERIFY(mSelectFile);
}

void UploadFileWidgetTest::shouldReturnEmptyResult()
{
    UploadFileWidget w;
    QVERIFY(w.description().isEmpty());
    QVERIFY(w.fileUrl().isEmpty());
}
