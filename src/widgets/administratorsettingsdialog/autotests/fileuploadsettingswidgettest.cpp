/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fileuploadsettingswidgettest.h"
#include "administratorsettingsdialog/fileupload/fileuploadsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QFormLayout>
#include <QSpinBox>
#include <QTest>
QTEST_MAIN(FileUploadSettingsWidgetTest)

FileUploadSettingsWidgetTest::FileUploadSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void FileUploadSettingsWidgetTest::shouldHaveDefaultValues()
{
    FileUploadSettingsWidget w(nullptr);
    QVERIFY(w.widgetResizable());

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mFileUploadsEnabled = w.findChild<QCheckBox *>(QStringLiteral("mFileUploadsEnabled"));
    QVERIFY(mFileUploadsEnabled);
    QVERIFY(!mFileUploadsEnabled->isChecked());
    QVERIFY(!mFileUploadsEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFileUploadsEnabled), QStringLiteral("FileUpload_Enabled"));

    auto mProtectUploadedFiles = w.findChild<QCheckBox *>(QStringLiteral("mProtectUploadedFiles"));
    QVERIFY(mProtectUploadedFiles);
    QVERIFY(!mProtectUploadedFiles->isChecked());
    QVERIFY(!mProtectUploadedFiles->text().isEmpty());
    QVERIFY(!mProtectUploadedFiles->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mProtectUploadedFiles), QStringLiteral("FileUpload_ProtectFiles"));

    auto mRotateImagesUpload = w.findChild<QCheckBox *>(QStringLiteral("mRotateImagesUpload"));
    QVERIFY(mRotateImagesUpload);
    QVERIFY(!mRotateImagesUpload->isChecked());
    QVERIFY(!mRotateImagesUpload->text().isEmpty());
    QVERIFY(!mRotateImagesUpload->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mRotateImagesUpload), QStringLiteral("FileUpload_RotateImages"));

    auto mMaximumFileUploadSize = w.findChild<QSpinBox *>(QStringLiteral("mMaximumFileUploadSize"));
    QVERIFY(mMaximumFileUploadSize);
    QVERIFY(!mMaximumFileUploadSize->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMaximumFileUploadSize), QStringLiteral("FileUpload_MaxFileSize"));

    auto mFileUploadsEnabledDirectMessages = w.findChild<QCheckBox *>(QStringLiteral("mFileUploadsEnabledDirectMessages"));
    QVERIFY(mFileUploadsEnabledDirectMessages);
    QVERIFY(!mFileUploadsEnabledDirectMessages->isChecked());
    QVERIFY(!mFileUploadsEnabledDirectMessages->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFileUploadsEnabledDirectMessages), QStringLiteral("FileUpload_Enabled_Direct"));
}
