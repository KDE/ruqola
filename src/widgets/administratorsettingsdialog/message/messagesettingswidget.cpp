/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messagesettingswidget.h"
#include <QCheckBox>
#include <QFormLayout>

#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>

MessageSettingsWidget::MessageSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mAllowMessageEditing(new QCheckBox(i18n("Allow Message Editing"), this))
    , mAllowMessageDeleting(new QCheckBox(i18n("Allow Message Deleting"), this))
    , mShowEditedStatus(new QCheckBox(i18n("Show Edited Status"), this))
    , mShowDeletedStatus(new QCheckBox(i18n("Show Deleted Status"), this))
    , mAllowMessagePinning(new QCheckBox(i18n("Allow Message Pinning"), this))
    , mAllowMessageSnippeting(new QCheckBox(i18n("Allow Message Snippeting"), this))
    , mAllowConvertingLongMessageAttachment(new QCheckBox(i18n("Allow converting long messages to attachment"), this))
    , mVideoRecorderEnabled(new QCheckBox(i18n("Video Recorder Enabled"), this))
    , mBlockMessageEditingAfterMinutes(new QSpinBox(this))
    , mBlockMessageDeletingAfterMinutes(new QSpinBox(this))
    , mMaximumAllowedCharactersPerMessage(new QSpinBox(this))
    , mSafePort(new QLineEdit(this))
{
    mAllowMessageEditing->setObjectName(QStringLiteral("mAllowMessageEditing"));
    mMainLayout->addWidget(mAllowMessageEditing);
    connectCheckBox(mAllowMessageEditing, QStringLiteral("Message_AllowEditing"));

    mBlockMessageEditingAfterMinutes->setObjectName(QStringLiteral("mBlockMessageEditingAfterMinutes"));
    mBlockMessageEditingAfterMinutes->setToolTip(i18n("Enter 0 to disable blocking."));
    addSpinbox(i18n("Block Message Editing After (n) Minutes"), mBlockMessageEditingAfterMinutes, QStringLiteral("Message_AllowEditing_BlockEditInMinutes"));

    mAllowMessageDeleting->setObjectName(QStringLiteral("mAllowMessageDeleting"));
    mMainLayout->addWidget(mAllowMessageDeleting);
    connectCheckBox(mAllowMessageDeleting, QStringLiteral("Message_AllowDeleting"));

    mBlockMessageDeletingAfterMinutes->setObjectName(QStringLiteral("mBlockMessageDeletingAfterMinutes"));
    mBlockMessageDeletingAfterMinutes->setToolTip(i18n("Block Message Deleting After (n) Minutes."));
    addSpinbox(i18n("Block Message Deleting After (n) Minutes"),
               mBlockMessageDeletingAfterMinutes,
               QStringLiteral("Message_AllowDeleting_BlockDeleteInMinutes"));

    mShowEditedStatus->setObjectName(QStringLiteral("mShowEditedStatus"));
    mMainLayout->addWidget(mShowEditedStatus);
    connectCheckBox(mShowEditedStatus, QStringLiteral("Message_ShowEditedStatus"));

    mShowDeletedStatus->setObjectName(QStringLiteral("mShowDeletedStatus"));
    mMainLayout->addWidget(mShowDeletedStatus);
    connectCheckBox(mShowDeletedStatus, QStringLiteral("Message_ShowDeletedStatus"));

    mAllowMessagePinning->setObjectName(QStringLiteral("mAllowMessagePinning"));
    mAllowMessagePinning->setToolTip(i18n("Allow Message Pinning"));
    mMainLayout->addWidget(mAllowMessagePinning);
    connectCheckBox(mAllowMessagePinning, QStringLiteral("Message_AllowPinning"));

    mAllowMessageSnippeting->setObjectName(QStringLiteral("mAllowMessageSnippeting"));
    mMainLayout->addWidget(mAllowMessageSnippeting);
    connectCheckBox(mAllowMessageSnippeting, QStringLiteral("Message_AllowSnippeting"));

    mAllowConvertingLongMessageAttachment->setObjectName(QStringLiteral("mAllowConvertingLongMessageAttachment"));
    mMainLayout->addWidget(mAllowConvertingLongMessageAttachment);
    connectCheckBox(mAllowConvertingLongMessageAttachment, QStringLiteral("Message_AllowConvertLongMessagesToAttachment"));

    mVideoRecorderEnabled->setObjectName(QStringLiteral("mVideoRecorderEnabled"));
    mVideoRecorderEnabled->setToolTip(i18n("Requires 'video/webm' files to be an accepted media type within 'File Upload' settings."));
    mMainLayout->addWidget(mVideoRecorderEnabled);
    connectCheckBox(mVideoRecorderEnabled, QStringLiteral("Message_VideoRecorderEnabled"));

    mMaximumAllowedCharactersPerMessage->setObjectName(QStringLiteral("mMaximumAllowedCharactersPerMessage"));
    mMaximumAllowedCharactersPerMessage->setMaximum(99999);
    addSpinbox(i18n("Maximum Allowed Characters Per Message"), mMaximumAllowedCharactersPerMessage, QStringLiteral("Message_MaxAllowedSize"));

    mSafePort->setObjectName(QStringLiteral("mSafePort"));
    addLineEdit(i18n("Safe Ports"), mSafePort, QStringLiteral("API_EmbedSafePorts"));

    auto messageAttachmentsLabel = createBoldLabel(i18n("Message Attachments"));
    messageAttachmentsLabel->setObjectName(QStringLiteral("messageAttachmentsLabel"));
    mMainLayout->addWidget(messageAttachmentsLabel);
}

MessageSettingsWidget::~MessageSettingsWidget() = default;

void MessageSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
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
    initializeWidget(mSafePort, mapSettings, QStringLiteral("80, 443"));
}
