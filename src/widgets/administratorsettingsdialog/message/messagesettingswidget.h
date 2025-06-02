/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QSpinBox;
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit MessageSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MessageSettingsWidget() override;

    void initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings) override;

private:
    QCheckBox *const mAllowMessageEditing;
    QSpinBox *const mBlockMessageEditingAfterMinutes;
    QCheckBox *const mAllowMessageDeleting;
    QSpinBox *const mBlockMessageDeletingAfterMinutes;
    QCheckBox *const mShowEditedStatus;
    QCheckBox *const mShowDeletedStatus;
    QCheckBox *const mAllowMessagePinning;
    QCheckBox *const mAllowMessageSnippeting;
    QCheckBox *const mAllowConvertingLongMessageAttachment;
    QCheckBox *const mVideoRecorderEnabled;
    QSpinBox *const mMaximumAllowedCharactersPerMessage;
    QLineEdit *const mSafePort;
    QCheckBox *const mRemoveEXIFMetadata;
    QCheckBox *const mEnableImageThumbnails;
    QSpinBox *const mThumbnailMaxWidth;
    QSpinBox *const mThumbnailMaxHeight;
    QCheckBox *const mAudioRecorderEnabled;
    QSpinBox *const mAudioMessageBitRate;
    QCheckBox *const mAutoTranslateEnabled;
    QComboBox *const mAutoTranslateServiceProvider;
    QLineEdit *const mDeeplApiKey;
    QLineEdit *const mGoogleApiKey;
    QLineEdit *const mMicrosoftApiKey;
    QSpinBox *const mGroupingPeriod;
    QSpinBox *const mDirectMessageMaxUsers;
    QSpinBox *const mMaximumNumberChainedQuotes;
    QComboBox *const mMessageErasureType;
    QCheckBox *const mApiEmbed;
    QSpinBox *const mEmbedCacheExpirationDays;
};
