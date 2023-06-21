/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userdatadownloadwidgettest.h"
#include "administratorsettingsdialog/userdatadownload/userdatadownloadwidget.h"
#include "settingswidgetshelper.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QTest>
QTEST_MAIN(UserDataDownloadWidgetTest)

UserDataDownloadWidgetTest::UserDataDownloadWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void UserDataDownloadWidgetTest::shouldHaveDefaultValues()
{
    UserDataDownloadWidget w(nullptr);
    QVERIFY(w.widgetResizable());

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mUserDataDownloadEnabled = w.findChild<QCheckBox *>(QStringLiteral("mUserDataDownloadEnabled"));
    QVERIFY(mUserDataDownloadEnabled);
    QVERIFY(!mUserDataDownloadEnabled->isChecked());
    QVERIFY(!mUserDataDownloadEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUserDataDownloadEnabled), QStringLiteral("UserData_EnableDownload"));

    auto mSystemPathExportedFiles = w.findChild<QLineEdit *>(QStringLiteral("mSystemPathExportedFiles"));
    QVERIFY(mSystemPathExportedFiles);
    QVERIFY(mSystemPathExportedFiles->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSystemPathExportedFiles), QStringLiteral("UserData_FileSystemPath"));

    auto mSystemPathCompressedFile = w.findChild<QLineEdit *>(QStringLiteral("mSystemPathCompressedFile"));
    QVERIFY(mSystemPathCompressedFile);
    QVERIFY(mSystemPathCompressedFile->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSystemPathCompressedFile), QStringLiteral("UserData_FileSystemZipPath"));

    auto mProcessingFrequency = w.findChild<QSpinBox *>(QStringLiteral("mProcessingFrequency"));
    QVERIFY(mProcessingFrequency);
    QCOMPARE(mProcessingFrequency->maximum(), 1000);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mProcessingFrequency), QStringLiteral("UserData_ProcessingFrequency"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("UserData_ProcessingFrequency"));

    auto mMessageLimitRequest = w.findChild<QSpinBox *>(QStringLiteral("mMessageLimitRequest"));
    QVERIFY(mMessageLimitRequest);
    QCOMPARE(mMessageLimitRequest->maximum(), 99999);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMessageLimitRequest), QStringLiteral("UserData_MessageLimitPerRequest"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("UserData_MessageLimitPerRequest"));
}

#include "moc_userdatadownloadwidgettest.cpp"
