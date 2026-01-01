/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messagesettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/message/messagesettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
QTEST_MAIN(MessageSettingsWidgetTest)

MessageSettingsWidgetTest::MessageSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MessageSettingsWidgetTest::shouldHaveDefaultValues()
{
    MessageSettingsWidget w(nullptr);
    QVERIFY(w.widgetResizable());

    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mAllowMessageEditing = w.findChild<QCheckBox *>(u"mAllowMessageEditing"_s);
    QVERIFY(mAllowMessageEditing);
    QVERIFY(!mAllowMessageEditing->isChecked());
    QVERIFY(!mAllowMessageEditing->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowMessageEditing), u"Message_AllowEditing"_s);

    auto mAllowMessageDeleting = w.findChild<QCheckBox *>(u"mAllowMessageDeleting"_s);
    QVERIFY(mAllowMessageDeleting);
    QVERIFY(!mAllowMessageDeleting->isChecked());
    QVERIFY(!mAllowMessageDeleting->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowMessageDeleting), u"Message_AllowDeleting"_s);

    auto mShowEditedStatus = w.findChild<QCheckBox *>(u"mShowEditedStatus"_s);
    QVERIFY(mShowEditedStatus);
    QVERIFY(!mShowEditedStatus->isChecked());
    QVERIFY(!mShowEditedStatus->text().isEmpty());
    QVERIFY(!mShowEditedStatus->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mShowEditedStatus), u"Message_ShowEditedStatus"_s);

    auto mShowDeletedStatus = w.findChild<QCheckBox *>(u"mShowDeletedStatus"_s);
    QVERIFY(mShowDeletedStatus);
    QVERIFY(!mShowDeletedStatus->isChecked());
    QVERIFY(!mShowDeletedStatus->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mShowDeletedStatus), u"Message_ShowDeletedStatus"_s);

    auto mAllowMessagePinning = w.findChild<QCheckBox *>(u"mAllowMessagePinning"_s);
    QVERIFY(mAllowMessagePinning);
    QVERIFY(!mAllowMessagePinning->isChecked());
    QVERIFY(!mAllowMessagePinning->text().isEmpty());
    QVERIFY(!mAllowMessagePinning->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowMessagePinning), u"Message_AllowPinning"_s);

    auto mAllowMessageSnippeting = w.findChild<QCheckBox *>(u"mAllowMessageSnippeting"_s);
    QVERIFY(mAllowMessageSnippeting);
    QVERIFY(!mAllowMessageSnippeting->isChecked());
    QVERIFY(!mAllowMessageSnippeting->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowMessageSnippeting), u"Message_AllowSnippeting"_s);

    auto mAllowConvertingLongMessageAttachment = w.findChild<QCheckBox *>(u"mAllowConvertingLongMessageAttachment"_s);
    QVERIFY(mAllowConvertingLongMessageAttachment);
    QVERIFY(!mAllowConvertingLongMessageAttachment->isChecked());
    QVERIFY(!mAllowConvertingLongMessageAttachment->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowConvertingLongMessageAttachment), u"Message_AllowConvertLongMessagesToAttachment"_s);

    auto mVideoRecorderEnabled = w.findChild<QCheckBox *>(u"mVideoRecorderEnabled"_s);
    QVERIFY(mVideoRecorderEnabled);
    QVERIFY(!mVideoRecorderEnabled->isChecked());
    QVERIFY(!mVideoRecorderEnabled->text().isEmpty());
    QVERIFY(!mVideoRecorderEnabled->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mVideoRecorderEnabled), u"Message_VideoRecorderEnabled"_s);

    auto mBlockMessageEditingAfterMinutes = w.findChild<QSpinBox *>(u"mBlockMessageEditingAfterMinutes"_s);
    QVERIFY(mBlockMessageEditingAfterMinutes);
    QVERIFY(!mBlockMessageEditingAfterMinutes->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mBlockMessageEditingAfterMinutes), u"Message_AllowEditing_BlockEditInMinutes"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Message_AllowEditing_BlockEditInMinutes"_s);

    auto mBlockMessageDeletingAfterMinutes = w.findChild<QSpinBox *>(u"mBlockMessageDeletingAfterMinutes"_s);
    QVERIFY(mBlockMessageDeletingAfterMinutes);
    QVERIFY(!mBlockMessageDeletingAfterMinutes->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mBlockMessageDeletingAfterMinutes), u"Message_AllowDeleting_BlockDeleteInMinutes"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Message_AllowDeleting_BlockDeleteInMinutes"_s);

    auto mMaximumAllowedCharactersPerMessage = w.findChild<QSpinBox *>(u"mMaximumAllowedCharactersPerMessage"_s);
    QVERIFY(mMaximumAllowedCharactersPerMessage);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMaximumAllowedCharactersPerMessage), u"Message_MaxAllowedSize"_s);
    QCOMPARE(mMaximumAllowedCharactersPerMessage->maximum(), 99999);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Message_MaxAllowedSize"_s);

    auto mSafePort = w.findChild<QLineEdit *>(u"mSafePort"_s);
    QVERIFY(mSafePort);
    QVERIFY(mSafePort->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSafePort), u"API_EmbedSafePorts"_s);

    auto messageAttachmentsLabel = w.findChild<QLabel *>(u"messageAttachmentsLabel"_s);
    QVERIFY(messageAttachmentsLabel);
    QVERIFY(!messageAttachmentsLabel->text().isEmpty());

    auto mRemoveEXIFMetadata = w.findChild<QCheckBox *>(u"mRemoveEXIFMetadata"_s);
    QVERIFY(mRemoveEXIFMetadata);
    QVERIFY(!mRemoveEXIFMetadata->isChecked());
    QVERIFY(!mRemoveEXIFMetadata->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mRemoveEXIFMetadata), u"Message_Attachments_Strip_Exif"_s);

    auto mEnableImageThumbnails = w.findChild<QCheckBox *>(u"mEnableImageThumbnails"_s);
    QVERIFY(mEnableImageThumbnails);
    QVERIFY(!mEnableImageThumbnails->isChecked());
    QVERIFY(!mEnableImageThumbnails->text().isEmpty());
    QVERIFY(!mEnableImageThumbnails->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableImageThumbnails), u"Message_Attachments_Thumbnails_Enabled"_s);

    auto audioMessageLabel = w.findChild<QLabel *>(u"audioMessageLabel"_s);
    QVERIFY(audioMessageLabel);
    QVERIFY(!audioMessageLabel->text().isEmpty());

    auto mThumbnailMaxWidth = w.findChild<QSpinBox *>(u"mThumbnailMaxWidth"_s);
    QVERIFY(mThumbnailMaxWidth);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mThumbnailMaxWidth), u"Message_Attachments_Thumbnails_Width"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Message_Attachments_Thumbnails_Width"_s);

    auto mThumbnailMaxHeight = w.findChild<QSpinBox *>(u"mThumbnailMaxHeight"_s);
    QVERIFY(mThumbnailMaxHeight);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mThumbnailMaxHeight), u"Message_Attachments_Thumbnails_Height"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Message_Attachments_Thumbnails_Height"_s);

    auto mAudioRecorderEnabled = w.findChild<QCheckBox *>(u"mAudioRecorderEnabled"_s);
    QVERIFY(mAudioRecorderEnabled);
    QVERIFY(!mAudioRecorderEnabled->isChecked());
    QVERIFY(!mAudioRecorderEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAudioRecorderEnabled), u"Message_AudioRecorderEnabled"_s);

    auto mAudioMessageBitRate = w.findChild<QSpinBox *>(u"mAudioMessageBitRate"_s);
    QVERIFY(mAudioMessageBitRate);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAudioMessageBitRate), u"Message_Audio_bitRate"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Message_Audio_bitRate"_s);

    auto autoTranslateLabel = w.findChild<QLabel *>(u"autoTranslateLabel"_s);
    QVERIFY(autoTranslateLabel);
    QVERIFY(!autoTranslateLabel->text().isEmpty());

    auto mAutoTranslateEnabled = w.findChild<QCheckBox *>(u"mAutoTranslateEnabled"_s);
    QVERIFY(mAutoTranslateEnabled);
    QVERIFY(!mAutoTranslateEnabled->isChecked());
    QVERIFY(!mAutoTranslateEnabled->text().isEmpty());
    QVERIFY(mAutoTranslateEnabled->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAutoTranslateEnabled), u"AutoTranslate_Enabled"_s);

    auto mAutoTranslateServiceProvider = w.findChild<QComboBox *>(u"mAutoTranslateServiceProvider"_s);
    QVERIFY(mAutoTranslateServiceProvider);
    QCOMPARE(mAutoTranslateServiceProvider->count(), 3);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAutoTranslateServiceProvider), u"AutoTranslate_ServiceProvider"_s);

    auto mDeeplApiKey = w.findChild<QLineEdit *>(u"mDeeplApiKey"_s);
    QVERIFY(mDeeplApiKey);
    QVERIFY(mDeeplApiKey->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDeeplApiKey), u"AutoTranslate_DeepLAPIKey"_s);

    auto mGoogleApiKey = w.findChild<QLineEdit *>(u"mGoogleApiKey"_s);
    QVERIFY(mGoogleApiKey);
    QVERIFY(mGoogleApiKey->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGoogleApiKey), u"AutoTranslate_GoogleAPIKey"_s);

    auto mMicrosoftApiKey = w.findChild<QLineEdit *>(u"mMicrosoftApiKey"_s);
    QVERIFY(mMicrosoftApiKey);
    QVERIFY(mMicrosoftApiKey->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMicrosoftApiKey), u"AutoTranslate_MicrosoftAPIKey"_s);

    auto mGroupingPeriod = w.findChild<QSpinBox *>(u"mGroupingPeriod"_s);
    QVERIFY(mGroupingPeriod);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGroupingPeriod), u"Message_GroupingPeriod"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Message_GroupingPeriod"_s);

    auto mDirectMessageMaxUsers = w.findChild<QSpinBox *>(u"mDirectMessageMaxUsers"_s);
    QVERIFY(mDirectMessageMaxUsers);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDirectMessageMaxUsers), u"DirectMesssage_maxUsers"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"DirectMesssage_maxUsers"_s);

    auto mMaximumNumberChainedQuotes = w.findChild<QSpinBox *>(u"mMaximumNumberChainedQuotes"_s);
    QVERIFY(mMaximumNumberChainedQuotes);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMaximumNumberChainedQuotes), u"Message_QuoteChainLimit"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Message_QuoteChainLimit"_s);

    auto mMessageErasureType = w.findChild<QComboBox *>(u"mMessageErasureType"_s);
    QVERIFY(mMessageErasureType);
    QCOMPARE(mMessageErasureType->count(), 3);
    QVERIFY(!mMessageErasureType->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMessageErasureType), u"Message_ErasureType"_s);

    auto mApiEmbed = w.findChild<QCheckBox *>(u"mApiEmbed"_s);
    QVERIFY(mApiEmbed);
    QVERIFY(!mApiEmbed->isChecked());
    QVERIFY(!mApiEmbed->text().isEmpty());
    QVERIFY(!mApiEmbed->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mApiEmbed), u"API_Embed"_s);

    auto mEmbedCacheExpirationDays = w.findChild<QSpinBox *>(u"mEmbedCacheExpirationDays"_s);
    QVERIFY(mEmbedCacheExpirationDays);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEmbedCacheExpirationDays), u"API_EmbedCacheExpirationDays"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"API_EmbedCacheExpirationDays"_s);
}

#include "moc_messagesettingswidgettest.cpp"
