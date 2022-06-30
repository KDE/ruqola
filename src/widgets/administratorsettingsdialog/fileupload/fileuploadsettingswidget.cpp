/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fileuploadsettingswidget.h"

#include <QFormLayout>

#include <KLocalizedString>
#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>

FileUploadSettingsWidget::FileUploadSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mFileUploadsEnabled(new QCheckBox(i18n("Enabled File Upload"), this))
    , mProtectUploadedFiles(new QCheckBox(i18n("Protect Uploaded Files"), this))
    , mRotateImagesUpload(new QCheckBox(i18n("Rotate images on upload"), this))
    , mMaximumFileUploadSize(new QSpinBox(this))
    , mFileUploadsEnabledDirectMessages(new QCheckBox(i18n("File Uploads Enabled in Direct Messages"), this))
    , mAcceptedMediaTypes(new QLineEdit(this))
    , mBlockedMediaTypes(new QLineEdit(this))
{
    mFileUploadsEnabled->setObjectName(QStringLiteral("mFileUploadsEnabled"));
    mMainLayout->addWidget(mFileUploadsEnabled);
    connectCheckBox(mFileUploadsEnabled, QStringLiteral("FileUpload_Enabled"));

    mMaximumFileUploadSize->setObjectName(QStringLiteral("mMaximumFileUploadSize"));
    mMaximumFileUploadSize->setToolTip(i18n("Set it to -1 to remove the file size limitation."));
    mMaximumFileUploadSize->setMaximum(1215752191);
    addSpinbox(i18n("Maximum File Upload Size (in bytes)"), mMaximumFileUploadSize, QStringLiteral("FileUpload_MaxFileSize"));

    mProtectUploadedFiles->setObjectName(QStringLiteral("mProtectUploadedFiles"));
    mProtectUploadedFiles->setToolTip(i18n("Only authenticated users will have access"));
    mMainLayout->addWidget(mProtectUploadedFiles);
    connectCheckBox(mProtectUploadedFiles, QStringLiteral("FileUpload_ProtectFiles"));

    mRotateImagesUpload->setObjectName(QStringLiteral("mRotateImagesUpload"));
    mRotateImagesUpload->setToolTip(i18n("Enabling this setting may cause image quality loss"));
    mMainLayout->addWidget(mRotateImagesUpload);
    connectCheckBox(mRotateImagesUpload, QStringLiteral("FileUpload_RotateImages"));

    mFileUploadsEnabledDirectMessages->setObjectName(QStringLiteral("mFileUploadsEnabledDirectMessages"));
    mMainLayout->addWidget(mFileUploadsEnabledDirectMessages);
    connectCheckBox(mFileUploadsEnabledDirectMessages, QStringLiteral("FileUpload_Enabled_Direct"));

    mAcceptedMediaTypes->setObjectName(QStringLiteral("mAcceptedMediaTypes"));
    mAcceptedMediaTypes->setToolTip(i18n("Comma-separated list of media types. Leave it blank for accepting all media types."));
    addLineEdit(i18n("Accepted Media Types"), mAcceptedMediaTypes, QStringLiteral("FileUpload_MediaTypeWhiteList"));

    mBlockedMediaTypes->setObjectName(QStringLiteral("mBlockedMediaTypes"));
    mBlockedMediaTypes->setToolTip(i18n("Comma-separated list of media types. This setting has priority over the Accepted Media Types."));
    addLineEdit(i18n("Accepted Media Types"), mBlockedMediaTypes, QStringLiteral("FileUpload_MediaTypeBlackList"));
}

FileUploadSettingsWidget::~FileUploadSettingsWidget() = default;

void FileUploadSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mFileUploadsEnabled, mapSettings);
    initializeWidget(mProtectUploadedFiles, mapSettings);
    initializeWidget(mRotateImagesUpload, mapSettings);
    initializeWidget(mMaximumFileUploadSize, mapSettings);
    initializeWidget(mFileUploadsEnabledDirectMessages, mapSettings);
    initializeWidget(mAcceptedMediaTypes, mapSettings);
    initializeWidget(mBlockedMediaTypes, mapSettings);
}
