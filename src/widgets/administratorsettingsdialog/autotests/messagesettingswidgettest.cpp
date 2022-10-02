/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messagesettingswidgettest.h"
#include "administratorsettingsdialog/message/messagesettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
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

    auto mAllowMessageEditing = w.findChild<QCheckBox *>(QStringLiteral("mAllowMessageEditing"));
    QVERIFY(mAllowMessageEditing);
    QVERIFY(!mAllowMessageEditing->isChecked());
    QVERIFY(!mAllowMessageEditing->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowMessageEditing), QStringLiteral("Message_AllowEditing"));

    auto mAllowMessageDeleting = w.findChild<QCheckBox *>(QStringLiteral("mAllowMessageDeleting"));
    QVERIFY(mAllowMessageDeleting);
    QVERIFY(!mAllowMessageDeleting->isChecked());
    QVERIFY(!mAllowMessageDeleting->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowMessageDeleting), QStringLiteral("Message_AllowDeleting"));

    auto mShowEditedStatus = w.findChild<QCheckBox *>(QStringLiteral("mShowEditedStatus"));
    QVERIFY(mShowEditedStatus);
    QVERIFY(!mShowEditedStatus->isChecked());
    QVERIFY(!mShowEditedStatus->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mShowEditedStatus), QStringLiteral("Message_ShowEditedStatus"));

    auto mShowDeletedStatus = w.findChild<QCheckBox *>(QStringLiteral("mShowDeletedStatus"));
    QVERIFY(mShowDeletedStatus);
    QVERIFY(!mShowDeletedStatus->isChecked());
    QVERIFY(!mShowDeletedStatus->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mShowDeletedStatus), QStringLiteral("Message_ShowDeletedStatus"));

    auto mAllowMessagePinning = w.findChild<QCheckBox *>(QStringLiteral("mAllowMessagePinning"));
    QVERIFY(mAllowMessagePinning);
    QVERIFY(!mAllowMessagePinning->isChecked());
    QVERIFY(!mAllowMessagePinning->text().isEmpty());
    QVERIFY(!mAllowMessagePinning->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowMessagePinning), QStringLiteral("Message_AllowPinning"));

    auto mAllowMessageSnippeting = w.findChild<QCheckBox *>(QStringLiteral("mAllowMessageSnippeting"));
    QVERIFY(mAllowMessageSnippeting);
    QVERIFY(!mAllowMessageSnippeting->isChecked());
    QVERIFY(!mAllowMessageSnippeting->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowMessageSnippeting), QStringLiteral("Message_AllowSnippeting"));

    auto mAllowConvertingLongMessageAttachment = w.findChild<QCheckBox *>(QStringLiteral("mAllowConvertingLongMessageAttachment"));
    QVERIFY(mAllowConvertingLongMessageAttachment);
    QVERIFY(!mAllowConvertingLongMessageAttachment->isChecked());
    QVERIFY(!mAllowConvertingLongMessageAttachment->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowConvertingLongMessageAttachment), QStringLiteral("Message_AllowConvertLongMessagesToAttachment"));

    auto mVideoRecorderEnabled = w.findChild<QCheckBox *>(QStringLiteral("mVideoRecorderEnabled"));
    QVERIFY(mVideoRecorderEnabled);
    QVERIFY(!mVideoRecorderEnabled->isChecked());
    QVERIFY(!mVideoRecorderEnabled->text().isEmpty());
    QVERIFY(!mVideoRecorderEnabled->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mVideoRecorderEnabled), QStringLiteral("Message_VideoRecorderEnabled"));

    auto mBlockMessageEditingAfterMinutes = w.findChild<QSpinBox *>(QStringLiteral("mBlockMessageEditingAfterMinutes"));
    QVERIFY(mBlockMessageEditingAfterMinutes);
    QVERIFY(!mBlockMessageEditingAfterMinutes->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mBlockMessageEditingAfterMinutes), QStringLiteral("Message_AllowEditing_BlockEditInMinutes"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Message_AllowEditing_BlockEditInMinutes"));

    auto mBlockMessageDeletingAfterMinutes = w.findChild<QSpinBox *>(QStringLiteral("mBlockMessageDeletingAfterMinutes"));
    QVERIFY(mBlockMessageDeletingAfterMinutes);
    QVERIFY(!mBlockMessageDeletingAfterMinutes->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mBlockMessageDeletingAfterMinutes), QStringLiteral("Message_AllowDeleting_BlockDeleteInMinutes"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Message_AllowDeleting_BlockDeleteInMinutes"));

    auto mMaximumAllowedCharactersPerMessage = w.findChild<QSpinBox *>(QStringLiteral("mMaximumAllowedCharactersPerMessage"));
    QVERIFY(mMaximumAllowedCharactersPerMessage);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMaximumAllowedCharactersPerMessage), QStringLiteral("Message_MaxAllowedSize"));
    QCOMPARE(mMaximumAllowedCharactersPerMessage->maximum(), 99999);
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Message_MaxAllowedSize"));

    auto mSafePort = w.findChild<QLineEdit *>(QStringLiteral("mSafePort"));
    QVERIFY(mSafePort);
    QVERIFY(mSafePort->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSafePort), QStringLiteral("API_EmbedSafePorts"));
}
