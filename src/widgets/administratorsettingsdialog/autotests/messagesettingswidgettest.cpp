/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messagesettingswidgettest.h"
#include "administratorsettingsdialog/message/messagesettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QTest>
QTEST_MAIN(MessageSettingsWidgetTest)

MessageSettingsWidgetTest::MessageSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MessageSettingsWidgetTest::shouldHaveDefaultValues()
{
    MessageSettingsWidget w(nullptr);
    QVERIFY(w.widgetResizable());

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAllowMessageEditing = w.findChild<QCheckBox *>(QStringLiteral("mAllowMessageEditing"));
    QVERIFY(mAllowMessageEditing);
    QVERIFY(!mAllowMessageEditing->isChecked());
    QVERIFY(!mAllowMessageEditing->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mAllowMessageEditing), QStringLiteral("Message_AllowEditing"));

    auto mAllowMessageDeleting = w.findChild<QCheckBox *>(QStringLiteral("mAllowMessageDeleting"));
    QVERIFY(mAllowMessageDeleting);
    QVERIFY(!mAllowMessageDeleting->isChecked());
    QVERIFY(!mAllowMessageDeleting->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mAllowMessageDeleting), QStringLiteral("Message_AllowDeleting"));

    auto mShowEditedStatus = w.findChild<QCheckBox *>(QStringLiteral("mShowEditedStatus"));
    QVERIFY(mShowEditedStatus);
    QVERIFY(!mShowEditedStatus->isChecked());
    QVERIFY(!mShowEditedStatus->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mShowEditedStatus), QStringLiteral("Message_ShowEditedStatus"));

    auto mShowDeletedStatus = w.findChild<QCheckBox *>(QStringLiteral("mShowDeletedStatus"));
    QVERIFY(mShowDeletedStatus);
    QVERIFY(!mShowDeletedStatus->isChecked());
    QVERIFY(!mShowDeletedStatus->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mShowDeletedStatus), QStringLiteral("Message_ShowDeletedStatus"));

    auto mAllowMessagePinning = w.findChild<QCheckBox *>(QStringLiteral("mAllowMessagePinning"));
    QVERIFY(mAllowMessagePinning);
    QVERIFY(!mAllowMessagePinning->isChecked());
    QVERIFY(!mAllowMessagePinning->text().isEmpty());
    QVERIFY(!mAllowMessagePinning->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mAllowMessagePinning), QStringLiteral("Message_AllowPinning"));

    auto mAllowMessageSnippeting = w.findChild<QCheckBox *>(QStringLiteral("mAllowMessageSnippeting"));
    QVERIFY(mAllowMessageSnippeting);
    QVERIFY(!mAllowMessageSnippeting->isChecked());
    QVERIFY(!mAllowMessageSnippeting->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mAllowMessageSnippeting), QStringLiteral("Message_AllowSnippeting"));

    auto mAllowConvertingLongMessageAttachment = w.findChild<QCheckBox *>(QStringLiteral("mAllowConvertingLongMessageAttachment"));
    QVERIFY(mAllowConvertingLongMessageAttachment);
    QVERIFY(!mAllowConvertingLongMessageAttachment->isChecked());
    QVERIFY(!mAllowConvertingLongMessageAttachment->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mAllowConvertingLongMessageAttachment), QStringLiteral("Message_AllowConvertLongMessagesToAttachment"));

    auto mVideoRecorderEnabled = w.findChild<QCheckBox *>(QStringLiteral("mVideoRecorderEnabled"));
    QVERIFY(mVideoRecorderEnabled);
    QVERIFY(!mVideoRecorderEnabled->isChecked());
    QVERIFY(!mVideoRecorderEnabled->text().isEmpty());
    QVERIFY(!mVideoRecorderEnabled->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mVideoRecorderEnabled), QStringLiteral("Message_VideoRecorderEnabled"));
}
