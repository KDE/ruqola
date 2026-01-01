/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messagesettingswidget.h"
using namespace Qt::Literals::StringLiterals;

#include <QCheckBox>
#include <QFormLayout>

#include <KLocalizedString>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>

MessageSettingsWidget::MessageSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mAllowMessageEditing(new QCheckBox(i18nc("@option:check", "Allow Message Editing"), this))
    , mBlockMessageEditingAfterMinutes(new QSpinBox(this))
    , mAllowMessageDeleting(new QCheckBox(i18nc("@option:check", "Allow Message Deleting"), this))
    , mBlockMessageDeletingAfterMinutes(new QSpinBox(this))
    , mShowEditedStatus(new QCheckBox(i18nc("@option:check", "Show Edited Status"), this))
    , mShowDeletedStatus(new QCheckBox(i18nc("@option:check", "Show Deleted Status"), this))
    , mAllowMessagePinning(new QCheckBox(i18nc("@option:check", "Allow Message Pinning"), this))
    , mAllowMessageSnippeting(new QCheckBox(i18nc("@option:check", "Allow Message Snippeting"), this))
    , mAllowConvertingLongMessageAttachment(new QCheckBox(i18nc("@option:check", "Allow converting long messages to attachment"), this))
    , mVideoRecorderEnabled(new QCheckBox(i18nc("@option:check", "Video Recorder Enabled"), this))
    , mMaximumAllowedCharactersPerMessage(new QSpinBox(this))
    , mSafePort(new QLineEdit(this))
    , mRemoveEXIFMetadata(new QCheckBox(i18nc("@option:check", "Remove EXIF metadata from supported files"), this))
    , mEnableImageThumbnails(new QCheckBox(i18nc("@option:check", "Enable image thumbnails to save bandwidth"), this))
    , mThumbnailMaxWidth(new QSpinBox(this))
    , mThumbnailMaxHeight(new QSpinBox(this))
    , mAudioRecorderEnabled(new QCheckBox(i18nc("@option:check", "Audio Recorder Enabled"), this))
    , mAudioMessageBitRate(new QSpinBox(this))
    , mAutoTranslateEnabled(new QCheckBox(i18nc("@option:check", "Enable Auto-Translate"), this))
    , mAutoTranslateServiceProvider(new QComboBox(this))
    , mDeeplApiKey(new QLineEdit(this))
    , mGoogleApiKey(new QLineEdit(this))
    , mMicrosoftApiKey(new QLineEdit(this))
    , mGroupingPeriod(new QSpinBox(this))
    , mDirectMessageMaxUsers(new QSpinBox(this))
    , mMaximumNumberChainedQuotes(new QSpinBox(this))
    , mMessageErasureType(new QComboBox(this))
    , mApiEmbed(new QCheckBox(i18nc("@option:check", "Embed Link Previews"), this))
    , mEmbedCacheExpirationDays(new QSpinBox(this))
{
    mAllowMessageEditing->setObjectName(u"mAllowMessageEditing"_s);
    addCheckBox(mAllowMessageEditing, u"Message_AllowEditing"_s);

    mBlockMessageEditingAfterMinutes->setObjectName(u"mBlockMessageEditingAfterMinutes"_s);
    mBlockMessageEditingAfterMinutes->setToolTip(i18nc("@info:tooltip", "Enter 0 to disable blocking."));
    addSpinbox(i18n("Block Message Editing After (n) Minutes"), mBlockMessageEditingAfterMinutes, u"Message_AllowEditing_BlockEditInMinutes"_s);

    mAllowMessageDeleting->setObjectName(u"mAllowMessageDeleting"_s);
    addCheckBox(mAllowMessageDeleting, u"Message_AllowDeleting"_s);

    mBlockMessageDeletingAfterMinutes->setObjectName(u"mBlockMessageDeletingAfterMinutes"_s);
    mBlockMessageDeletingAfterMinutes->setToolTip(i18nc("@info:tooltip", "Block Message Deleting After (n) Minutes."));
    addSpinbox(i18n("Block Message Deleting After (n) Minutes"), mBlockMessageDeletingAfterMinutes, u"Message_AllowDeleting_BlockDeleteInMinutes"_s);

    mShowEditedStatus->setObjectName(u"mShowEditedStatus"_s);
    mShowEditedStatus->setToolTip(i18nc("@info:tooltip", "This is a deprecated feature. It may not work as expected and will not get new updates."));
    addCheckBox(mShowEditedStatus, u"Message_ShowEditedStatus"_s);

    mShowDeletedStatus->setObjectName(u"mShowDeletedStatus"_s);
    addCheckBox(mShowDeletedStatus, u"Message_ShowDeletedStatus"_s);

    mAllowMessagePinning->setObjectName(u"mAllowMessagePinning"_s);
    mAllowMessagePinning->setToolTip(i18nc("@info:tooltip", "Allow Message Pinning"));
    addCheckBox(mAllowMessagePinning, u"Message_AllowPinning"_s);

    mAllowMessageSnippeting->setObjectName(u"mAllowMessageSnippeting"_s);
    addCheckBox(mAllowMessageSnippeting, u"Message_AllowSnippeting"_s);

    mAllowConvertingLongMessageAttachment->setObjectName(u"mAllowConvertingLongMessageAttachment"_s);
    addCheckBox(mAllowConvertingLongMessageAttachment, u"Message_AllowConvertLongMessagesToAttachment"_s);

    mVideoRecorderEnabled->setObjectName(u"mVideoRecorderEnabled"_s);
    mVideoRecorderEnabled->setToolTip(i18nc("@info:tooltip", "Requires 'video/webm' files to be an accepted media type within 'File Upload' settings."));
    addCheckBox(mVideoRecorderEnabled, u"Message_VideoRecorderEnabled"_s);

    mMaximumAllowedCharactersPerMessage->setObjectName(u"mMaximumAllowedCharactersPerMessage"_s);
    mMaximumAllowedCharactersPerMessage->setMaximum(99999);
    addSpinbox(i18n("Maximum Allowed Characters Per Message"), mMaximumAllowedCharactersPerMessage, u"Message_MaxAllowedSize"_s);

    mSafePort->setObjectName(u"mSafePort"_s);
    addLineEdit(i18n("Safe Ports"), mSafePort, u"API_EmbedSafePorts"_s);

    mMaximumNumberChainedQuotes->setObjectName(u"mMaximumNumberChainedQuotes"_s);
    addSpinbox(i18n("Maximum Number of Chained Quotes"), mMaximumNumberChainedQuotes, u"Message_QuoteChainLimit"_s);

    mMessageErasureType->setObjectName(u"mMessageErasureType"_s);
    const QMap<QString, QString> messageErasureTypeMaps = {
        {u"Keep"_s, i18n("Keep Messages and User Name")},
        {u"Delete"_s, i18n("Delete All Messages")},
        {u"Unlink"_s, i18n("Remove Link Between User and Messages")},
    };
    mMessageErasureType->setToolTip(
        i18n("Determine what to do with messages of users who remove their account. Keep Messages and User Name: The message and files history of the user "
             "will be deleted from Direct Messages and will be kept in other rooms. Delete All Messages: All messages and files from the user will be deleted "
             "from the database and it will not be possible to locate the user anymore. Remove link between user and messages: This option will assign all "
             "messages and files of the user to Rocket.Chat bot and Direct Messages are going to be deleted."));
    addComboBox(i18n("Message Erasure Type"), messageErasureTypeMaps, mMessageErasureType, u"Message_ErasureType"_s);

    mGroupingPeriod->setObjectName(u"mGroupingPeriod"_s);
    mGroupingPeriod->setToolTip(
        i18n("Messages will be grouped with previous message if both are from the same user and the elapsed time was less than the informed time in seconds."));
    addSpinbox(i18n("Grouping Period (in seconds)"), mGroupingPeriod, u"Message_GroupingPeriod"_s);

    mDirectMessageMaxUsers->setObjectName(u"mDirectMessageMaxUsers"_s);
    // Typo in RC => 3 "s"
    addSpinbox(i18n("Direct Message Maximum Users"), mDirectMessageMaxUsers, u"DirectMesssage_maxUsers"_s);

    auto messageAttachmentsLabel = createBoldLabel(i18n("Message Attachments"));
    messageAttachmentsLabel->setObjectName(u"messageAttachmentsLabel"_s);
    mMainLayout->addWidget(messageAttachmentsLabel);

    mRemoveEXIFMetadata->setObjectName(u"mRemoveEXIFMetadata"_s);
    mRemoveEXIFMetadata->setToolTip(i18nc(
        "@info:tooltip",
        "Strips out EXIF metadata from image files (jpeg, tiff, etc). This setting is not retroactive, so files uploaded while disabled will have EXIF data"));
    addCheckBox(mRemoveEXIFMetadata, u"Message_Attachments_Strip_Exif"_s);

    mEnableImageThumbnails->setObjectName(u"mEnableImageThumbnails"_s);
    mEnableImageThumbnails->setToolTip(i18nc(
        "@info:tooltip",
        "Thumbnails will be served instead of the original image to reduce bandwidth usage. Images at original resolution can be downloaded using the icon "
        "next to the attachment's name."));
    addCheckBox(mEnableImageThumbnails, u"Message_Attachments_Thumbnails_Enabled"_s);

    mThumbnailMaxWidth->setObjectName(u"mThumbnailMaxWidth"_s);
    addSpinbox(i18n("Thumbnail's max width (in pixels)"), mThumbnailMaxWidth, u"Message_Attachments_Thumbnails_Width"_s);

    mThumbnailMaxHeight->setObjectName(u"mThumbnailMaxHeight"_s);
    addSpinbox(i18n("Thumbnail's max height (in pixels)"), mThumbnailMaxHeight, u"Message_Attachments_Thumbnails_Height"_s);

    auto audioMessageLabel = createBoldLabel(i18n("Audio Message"));
    audioMessageLabel->setObjectName(u"audioMessageLabel"_s);
    mMainLayout->addWidget(audioMessageLabel);

    mAudioRecorderEnabled->setObjectName(u"mAudioRecorderEnabled"_s);
    addCheckBox(mAudioRecorderEnabled, u"Message_AudioRecorderEnabled"_s);

    mAudioMessageBitRate->setObjectName(u"mAudioMessageBitRate"_s);
    mAudioMessageBitRate->setMaximum(1000);
    addSpinbox(i18n("Audio Message Bit Rate"), mAudioMessageBitRate, u"Message_Audio_bitRate"_s);

    auto autoTranslateLabel = createBoldLabel(i18n("Auto-Translate"));
    autoTranslateLabel->setObjectName(u"autoTranslateLabel"_s);
    mMainLayout->addWidget(autoTranslateLabel);

    mAutoTranslateEnabled->setObjectName(u"mAutoTranslateEnabled"_s);
    addCheckBox(mAutoTranslateEnabled, u"AutoTranslate_Enabled"_s);

    mAutoTranslateServiceProvider->setObjectName(u"mAutoTranslateServiceProvider"_s);
    const QMap<QString, QString> maps = {
        {u"google-translate"_s, i18n("Google")},
        {u"deepl-translate"_s, i18n("Deepl")},
        {u"microsoft-translate"_s, i18n("Microsoft")},
    };
    addComboBox(i18n("Service Provider"), maps, mAutoTranslateServiceProvider, u"AutoTranslate_ServiceProvider"_s);

    mDeeplApiKey->setObjectName(u"mDeeplApiKey"_s);
    addLineEdit(i18n("DeepL API Key"), mDeeplApiKey, u"AutoTranslate_DeepLAPIKey"_s);

    mGoogleApiKey->setObjectName(u"mGoogleApiKey"_s);
    addLineEdit(i18n("Google API Key"), mGoogleApiKey, u"AutoTranslate_GoogleAPIKey"_s);

    mMicrosoftApiKey->setObjectName(u"mMicrosoftApiKey"_s);
    addLineEdit(i18n("Microsoft API Key"), mMicrosoftApiKey, u"AutoTranslate_MicrosoftAPIKey"_s);

    mApiEmbed->setObjectName(u"mApiEmbed"_s);
    mApiEmbed->setToolTip(i18nc("@info:tooltip", "Whether embedded link previews are enabled or not when a user posts a link to a website."));
    mMainLayout->addWidget(mApiEmbed);
    addCheckBox(mApiEmbed, u"API_Embed"_s);

    mEmbedCacheExpirationDays->setObjectName(u"mEmbedCacheExpirationDays"_s);
    addSpinbox(i18n("Embed Cache Expiration Days"), mEmbedCacheExpirationDays, u"API_EmbedCacheExpirationDays"_s);
}

MessageSettingsWidget::~MessageSettingsWidget() = default;

void MessageSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mAllowMessageEditing, mapSettings, true);
    initializeWidget(mAllowMessageDeleting, mapSettings, true);
    initializeWidget(mShowEditedStatus, mapSettings, true);
    initializeWidget(mShowDeletedStatus, mapSettings, false);
    initializeWidget(mAllowMessagePinning, mapSettings, true);
    initializeWidget(mAllowMessageSnippeting, mapSettings, false);
    initializeWidget(mAllowConvertingLongMessageAttachment, mapSettings, true);
    initializeWidget(mVideoRecorderEnabled, mapSettings, true);
    initializeWidget(mBlockMessageEditingAfterMinutes, mapSettings, 0);
    initializeWidget(mBlockMessageDeletingAfterMinutes, mapSettings, 0);
    initializeWidget(mMaximumAllowedCharactersPerMessage, mapSettings, 5000);
    initializeWidget(mSafePort, mapSettings, u"80, 443"_s);
    initializeWidget(mThumbnailMaxHeight, mapSettings, false);
    initializeWidget(mEnableImageThumbnails, mapSettings, true);
    initializeWidget(mThumbnailMaxWidth, mapSettings, 480);
    initializeWidget(mThumbnailMaxHeight, mapSettings, 360);
    initializeWidget(mAudioRecorderEnabled, mapSettings, true);
    initializeWidget(mAudioMessageBitRate, mapSettings, 32);
    initializeWidget(mAutoTranslateEnabled, mapSettings, false);
    initializeWidget(mAutoTranslateServiceProvider, mapSettings, u"google-translate"_s);
    initializeWidget(mDeeplApiKey, mapSettings, {});
    initializeWidget(mGoogleApiKey, mapSettings, {});
    initializeWidget(mMicrosoftApiKey, mapSettings, {});
    initializeWidget(mGroupingPeriod, mapSettings, 300);
    initializeWidget(mDirectMessageMaxUsers, mapSettings, 8);
    initializeWidget(mMaximumNumberChainedQuotes, mapSettings, 2);
    initializeWidget(mMessageErasureType, mapSettings, u"Delete"_s);
    initializeWidget(mApiEmbed, mapSettings, true);
    initializeWidget(mEmbedCacheExpirationDays, mapSettings, 30);
}

#include "moc_messagesettingswidget.cpp"
