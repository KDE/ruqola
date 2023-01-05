/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messagesettingswidgettest.h"
#include "administratorsettingsdialog/message/messagesettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
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
    QVERIFY(!mShowEditedStatus->toolTip().isEmpty());
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

    auto messageAttachmentsLabel = w.findChild<QLabel *>(QStringLiteral("messageAttachmentsLabel"));
    QVERIFY(messageAttachmentsLabel);
    QVERIFY(!messageAttachmentsLabel->text().isEmpty());

    auto mRemoveEXIFMetadata = w.findChild<QCheckBox *>(QStringLiteral("mRemoveEXIFMetadata"));
    QVERIFY(mRemoveEXIFMetadata);
    QVERIFY(!mRemoveEXIFMetadata->isChecked());
    QVERIFY(!mRemoveEXIFMetadata->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mRemoveEXIFMetadata), QStringLiteral("Message_Attachments_Strip_Exif"));

    auto mEnableImageThumbnails = w.findChild<QCheckBox *>(QStringLiteral("mEnableImageThumbnails"));
    QVERIFY(mEnableImageThumbnails);
    QVERIFY(!mEnableImageThumbnails->isChecked());
    QVERIFY(!mEnableImageThumbnails->text().isEmpty());
    QVERIFY(!mEnableImageThumbnails->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableImageThumbnails), QStringLiteral("Message_Attachments_Thumbnails_Enabled"));

    auto audioMessageLabel = w.findChild<QLabel *>(QStringLiteral("audioMessageLabel"));
    QVERIFY(audioMessageLabel);
    QVERIFY(!audioMessageLabel->text().isEmpty());

    auto mThumbnailMaxWidth = w.findChild<QSpinBox *>(QStringLiteral("mThumbnailMaxWidth"));
    QVERIFY(mThumbnailMaxWidth);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mThumbnailMaxWidth), QStringLiteral("Message_Attachments_Thumbnails_Width"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Message_Attachments_Thumbnails_Width"));

    auto mThumbnailMaxHeight = w.findChild<QSpinBox *>(QStringLiteral("mThumbnailMaxHeight"));
    QVERIFY(mThumbnailMaxHeight);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mThumbnailMaxHeight), QStringLiteral("Message_Attachments_Thumbnails_Height"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Message_Attachments_Thumbnails_Height"));

    auto mAudioRecorderEnabled = w.findChild<QCheckBox *>(QStringLiteral("mAudioRecorderEnabled"));
    QVERIFY(mAudioRecorderEnabled);
    QVERIFY(!mAudioRecorderEnabled->isChecked());
    QVERIFY(!mAudioRecorderEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAudioRecorderEnabled), QStringLiteral("Message_AudioRecorderEnabled"));

    auto mAudioMessageBitRate = w.findChild<QSpinBox *>(QStringLiteral("mAudioMessageBitRate"));
    QVERIFY(mAudioMessageBitRate);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAudioMessageBitRate), QStringLiteral("Message_Audio_bitRate"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Message_Audio_bitRate"));

    auto autoTranslateLabel = w.findChild<QLabel *>(QStringLiteral("autoTranslateLabel"));
    QVERIFY(autoTranslateLabel);
    QVERIFY(!autoTranslateLabel->text().isEmpty());

    auto mAutoTranslateEnabled = w.findChild<QCheckBox *>(QStringLiteral("mAutoTranslateEnabled"));
    QVERIFY(mAutoTranslateEnabled);
    QVERIFY(!mAutoTranslateEnabled->isChecked());
    QVERIFY(!mAutoTranslateEnabled->text().isEmpty());
    QVERIFY(mAutoTranslateEnabled->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAutoTranslateEnabled), QStringLiteral("AutoTranslate_Enabled"));

    auto mAutoTranslateServiceProvider = w.findChild<QComboBox *>(QStringLiteral("mAutoTranslateServiceProvider"));
    QVERIFY(mAutoTranslateServiceProvider);
    QCOMPARE(mAutoTranslateServiceProvider->count(), 3);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAutoTranslateServiceProvider), QStringLiteral("AutoTranslate_ServiceProvider"));

    auto mDeeplApiKey = w.findChild<QLineEdit *>(QStringLiteral("mDeeplApiKey"));
    QVERIFY(mDeeplApiKey);
    QVERIFY(mDeeplApiKey->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDeeplApiKey), QStringLiteral("AutoTranslate_DeepLAPIKey"));

    auto mGoogleApiKey = w.findChild<QLineEdit *>(QStringLiteral("mGoogleApiKey"));
    QVERIFY(mGoogleApiKey);
    QVERIFY(mGoogleApiKey->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGoogleApiKey), QStringLiteral("AutoTranslate_GoogleAPIKey"));

    auto mMicrosoftApiKey = w.findChild<QLineEdit *>(QStringLiteral("mMicrosoftApiKey"));
    QVERIFY(mMicrosoftApiKey);
    QVERIFY(mMicrosoftApiKey->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMicrosoftApiKey), QStringLiteral("AutoTranslate_MicrosoftAPIKey"));

    auto mGroupingPeriod = w.findChild<QSpinBox *>(QStringLiteral("mGroupingPeriod"));
    QVERIFY(mGroupingPeriod);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGroupingPeriod), QStringLiteral("Message_GroupingPeriod"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Message_GroupingPeriod"));
}
