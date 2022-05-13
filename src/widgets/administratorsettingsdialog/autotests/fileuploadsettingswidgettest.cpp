/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fileuploadsettingswidgettest.h"
#include "administratorsettingsdialog/fileupload/fileuploadsettingswidget.h"
#include <QFormLayout>
#include <QTest>
QTEST_MAIN(FileUploadSettingsWidgetTest)

FileUploadSettingsWidgetTest::FileUploadSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void FileUploadSettingsWidgetTest::shouldHaveDefaultValues()
{
    FileUploadSettingsWidget w(nullptr);
    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
}
