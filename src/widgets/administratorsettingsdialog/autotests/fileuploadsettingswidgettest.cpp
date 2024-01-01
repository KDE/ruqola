/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fileuploadsettingswidgettest.h"
#include "administratorsettingsdialog/fileupload/fileuploadsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
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

    auto mRestrictFilesToRooms = w.findChild<QCheckBox *>(QStringLiteral("mRestrictFilesToRooms"));
    QVERIFY(mRestrictFilesToRooms);
    QVERIFY(!mRestrictFilesToRooms->isChecked());
    QVERIFY(!mRestrictFilesToRooms->text().isEmpty());
    QVERIFY(!mRestrictFilesToRooms->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mRestrictFilesToRooms), QStringLiteral("FileUpload_Restrict_to_room_members"));

    auto mMaximumFileUploadSize = w.findChild<QSpinBox *>(QStringLiteral("mMaximumFileUploadSize"));
    QVERIFY(mMaximumFileUploadSize);
    QVERIFY(!mMaximumFileUploadSize->toolTip().isEmpty());
    QCOMPARE(mMaximumFileUploadSize->maximum(), 1215752191);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMaximumFileUploadSize), QStringLiteral("FileUpload_MaxFileSize"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("FileUpload_MaxFileSize"));

    auto mFileUploadsEnabledDirectMessages = w.findChild<QCheckBox *>(QStringLiteral("mFileUploadsEnabledDirectMessages"));
    QVERIFY(mFileUploadsEnabledDirectMessages);
    QVERIFY(!mFileUploadsEnabledDirectMessages->isChecked());
    QVERIFY(!mFileUploadsEnabledDirectMessages->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFileUploadsEnabledDirectMessages), QStringLiteral("FileUpload_Enabled_Direct"));

    auto mAcceptedMediaTypes = w.findChild<QLineEdit *>(QStringLiteral("mAcceptedMediaTypes"));
    QVERIFY(mAcceptedMediaTypes);
    QVERIFY(mAcceptedMediaTypes->text().isEmpty());
    QVERIFY(!mAcceptedMediaTypes->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAcceptedMediaTypes), QStringLiteral("FileUpload_MediaTypeWhiteList"));

    auto mBlockedMediaTypes = w.findChild<QLineEdit *>(QStringLiteral("mBlockedMediaTypes"));
    QVERIFY(mBlockedMediaTypes);
    QVERIFY(mBlockedMediaTypes->text().isEmpty());
    QVERIFY(!mBlockedMediaTypes->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mBlockedMediaTypes), QStringLiteral("FileUpload_MediaTypeBlackList"));

    auto mFileUploadJsonWebTokenSecret = w.findChild<QLineEdit *>(QStringLiteral("mFileUploadJsonWebTokenSecret"));
    QVERIFY(mFileUploadJsonWebTokenSecret);
    QVERIFY(mFileUploadJsonWebTokenSecret->text().isEmpty());
    QVERIFY(!mFileUploadJsonWebTokenSecret->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFileUploadJsonWebTokenSecret), QStringLiteral("FileUpload_json_web_token_secret_for_files"));

    auto mStorageType = w.findChild<QComboBox *>(QStringLiteral("mStorageType"));
    QVERIFY(mStorageType);
    QCOMPARE(mStorageType->count(), 5);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mStorageType), QStringLiteral("FileUpload_Storage_Type"));
}

#include "moc_fileuploadsettingswidgettest.cpp"
