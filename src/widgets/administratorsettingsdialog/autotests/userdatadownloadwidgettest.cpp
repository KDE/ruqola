/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userdatadownloadwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/userdatadownload/userdatadownloadwidget.h"
#include "settingswidgetshelper.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
QTEST_MAIN(UserDataDownloadWidgetTest)

UserDataDownloadWidgetTest::UserDataDownloadWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void UserDataDownloadWidgetTest::shouldHaveDefaultValues()
{
    UserDataDownloadWidget w(nullptr);
    QVERIFY(w.widgetResizable());

    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mUserDataDownloadEnabled = w.findChild<QCheckBox *>(u"mUserDataDownloadEnabled"_s);
    QVERIFY(mUserDataDownloadEnabled);
    QVERIFY(!mUserDataDownloadEnabled->isChecked());
    QVERIFY(!mUserDataDownloadEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUserDataDownloadEnabled), u"UserData_EnableDownload"_s);

    auto mSystemPathExportedFiles = w.findChild<QLineEdit *>(u"mSystemPathExportedFiles"_s);
    QVERIFY(mSystemPathExportedFiles);
    QVERIFY(mSystemPathExportedFiles->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSystemPathExportedFiles), u"UserData_FileSystemPath"_s);

    auto mSystemPathCompressedFile = w.findChild<QLineEdit *>(u"mSystemPathCompressedFile"_s);
    QVERIFY(mSystemPathCompressedFile);
    QVERIFY(mSystemPathCompressedFile->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSystemPathCompressedFile), u"UserData_FileSystemZipPath"_s);

    auto mProcessingFrequency = w.findChild<QSpinBox *>(u"mProcessingFrequency"_s);
    QVERIFY(mProcessingFrequency);
    QCOMPARE(mProcessingFrequency->maximum(), 1000);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mProcessingFrequency), u"UserData_ProcessingFrequency"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"UserData_ProcessingFrequency"_s);

    auto mMessageLimitRequest = w.findChild<QSpinBox *>(u"mMessageLimitRequest"_s);
    QVERIFY(mMessageLimitRequest);
    QCOMPARE(mMessageLimitRequest->maximum(), 99999);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMessageLimitRequest), u"UserData_MessageLimitPerRequest"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"UserData_MessageLimitPerRequest"_s);
}

#include "moc_userdatadownloadwidgettest.cpp"
