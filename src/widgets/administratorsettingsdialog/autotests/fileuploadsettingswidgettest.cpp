/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fileuploadsettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mFileUploadsEnabled = w.findChild<QCheckBox *>(u"mFileUploadsEnabled"_s);
    QVERIFY(mFileUploadsEnabled);
    QVERIFY(!mFileUploadsEnabled->isChecked());
    QVERIFY(!mFileUploadsEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFileUploadsEnabled), u"FileUpload_Enabled"_s);

    auto mProtectUploadedFiles = w.findChild<QCheckBox *>(u"mProtectUploadedFiles"_s);
    QVERIFY(mProtectUploadedFiles);
    QVERIFY(!mProtectUploadedFiles->isChecked());
    QVERIFY(!mProtectUploadedFiles->text().isEmpty());
    QVERIFY(!mProtectUploadedFiles->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mProtectUploadedFiles), u"FileUpload_ProtectFiles"_s);

    auto mRotateImagesUpload = w.findChild<QCheckBox *>(u"mRotateImagesUpload"_s);
    QVERIFY(mRotateImagesUpload);
    QVERIFY(!mRotateImagesUpload->isChecked());
    QVERIFY(!mRotateImagesUpload->text().isEmpty());
    QVERIFY(!mRotateImagesUpload->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mRotateImagesUpload), u"FileUpload_RotateImages"_s);

    auto mRestrictFilesToRooms = w.findChild<QCheckBox *>(u"mRestrictFilesToRooms"_s);
    QVERIFY(mRestrictFilesToRooms);
    QVERIFY(!mRestrictFilesToRooms->isChecked());
    QVERIFY(!mRestrictFilesToRooms->text().isEmpty());
    QVERIFY(!mRestrictFilesToRooms->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mRestrictFilesToRooms), u"FileUpload_Restrict_to_room_members"_s);

    auto mMaximumFileUploadSize = w.findChild<QSpinBox *>(u"mMaximumFileUploadSize"_s);
    QVERIFY(mMaximumFileUploadSize);
    QVERIFY(!mMaximumFileUploadSize->toolTip().isEmpty());
    QCOMPARE(mMaximumFileUploadSize->maximum(), 1215752191);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMaximumFileUploadSize), u"FileUpload_MaxFileSize"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"FileUpload_MaxFileSize"_s);

    auto mFileUploadsEnabledDirectMessages = w.findChild<QCheckBox *>(u"mFileUploadsEnabledDirectMessages"_s);
    QVERIFY(mFileUploadsEnabledDirectMessages);
    QVERIFY(!mFileUploadsEnabledDirectMessages->isChecked());
    QVERIFY(!mFileUploadsEnabledDirectMessages->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFileUploadsEnabledDirectMessages), u"FileUpload_Enabled_Direct"_s);

    auto mAcceptedMediaTypes = w.findChild<QLineEdit *>(u"mAcceptedMediaTypes"_s);
    QVERIFY(mAcceptedMediaTypes);
    QVERIFY(mAcceptedMediaTypes->text().isEmpty());
    QVERIFY(!mAcceptedMediaTypes->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAcceptedMediaTypes), u"FileUpload_MediaTypeWhiteList"_s);

    auto mBlockedMediaTypes = w.findChild<QLineEdit *>(u"mBlockedMediaTypes"_s);
    QVERIFY(mBlockedMediaTypes);
    QVERIFY(mBlockedMediaTypes->text().isEmpty());
    QVERIFY(!mBlockedMediaTypes->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mBlockedMediaTypes), u"FileUpload_MediaTypeBlackList"_s);

    auto mFileUploadJsonWebTokenSecret = w.findChild<QLineEdit *>(u"mFileUploadJsonWebTokenSecret"_s);
    QVERIFY(mFileUploadJsonWebTokenSecret);
    QVERIFY(mFileUploadJsonWebTokenSecret->text().isEmpty());
    QVERIFY(!mFileUploadJsonWebTokenSecret->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFileUploadJsonWebTokenSecret), u"FileUpload_json_web_token_secret_for_files"_s);

    auto mStorageType = w.findChild<QComboBox *>(u"mStorageType"_s);
    QVERIFY(mStorageType);
    QCOMPARE(mStorageType->count(), 5);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mStorageType), u"FileUpload_Storage_Type"_s);
}

#include "moc_fileuploadsettingswidgettest.cpp"
