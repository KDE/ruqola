/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fileuploadsettingswidgettest.h"
#include "administratorsettingsdialog/fileupload/fileuploadsettingswidget.h"
#include "settingswidgetshelper.h"
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

    auto mFileUploadsEnabled = w.findChild<QCheckBox *>(QStringLiteral("mFileUploadsEnabled"));
    QVERIFY(mFileUploadsEnabled);
    QVERIFY(!mFileUploadsEnabled->isChecked());
    QVERIFY(!mFileUploadsEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mFileUploadsEnabled), QStringLiteral("FileUpload_Enabled"));

    auto mProtectUploadedFiles = w.findChild<QCheckBox *>(QStringLiteral("mProtectUploadedFiles"));
    QVERIFY(mProtectUploadedFiles);
    QVERIFY(!mProtectUploadedFiles->isChecked());
    QVERIFY(!mProtectUploadedFiles->text().isEmpty());
    QVERIFY(!mProtectUploadedFiles->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mProtectUploadedFiles), QStringLiteral("FileUpload_ProtectFiles"));
}
