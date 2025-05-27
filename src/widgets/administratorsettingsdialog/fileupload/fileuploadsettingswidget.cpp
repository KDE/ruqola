/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fileuploadsettingswidget.h"

#include <QFormLayout>

#include <KLocalizedString>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>

FileUploadSettingsWidget::FileUploadSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mFileUploadsEnabled(new QCheckBox(i18nc("@option:check", "Enabled File Upload"), this))
    , mMaximumFileUploadSize(new QSpinBox(this))
    , mProtectUploadedFiles(new QCheckBox(i18nc("@option:check", "Protect Uploaded Files"), this))
    , mRotateImagesUpload(new QCheckBox(i18nc("@option:check", "Rotate images on upload"), this))
    , mRestrictFilesToRooms(new QCheckBox(i18nc("@option:check", "Restrict files to rooms' members"), this))
    , mFileUploadsEnabledDirectMessages(new QCheckBox(i18nc("@option:check", "File Uploads Enabled in Direct Messages"), this))
    , mAcceptedMediaTypes(new QLineEdit(this))
    , mBlockedMediaTypes(new QLineEdit(this))
    , mFileUploadJsonWebTokenSecret(new QLineEdit(this))
    , mStorageType(new QComboBox(this))
{
    mFileUploadsEnabled->setObjectName(QStringLiteral("mFileUploadsEnabled"));
    addCheckBox(mFileUploadsEnabled, QStringLiteral("FileUpload_Enabled"));

    mMaximumFileUploadSize->setObjectName(QStringLiteral("mMaximumFileUploadSize"));
    mMaximumFileUploadSize->setToolTip(i18nc("@info:tooltip", "Set it to -1 to remove the file size limitation."));
    mMaximumFileUploadSize->setMaximum(1215752191);
    addSpinbox(i18n("Maximum File Upload Size (in bytes)"), mMaximumFileUploadSize, QStringLiteral("FileUpload_MaxFileSize"));

    mProtectUploadedFiles->setObjectName(QStringLiteral("mProtectUploadedFiles"));
    mProtectUploadedFiles->setToolTip(i18nc("@info:tooltip", "Only authenticated users will have access"));
    addCheckBox(mProtectUploadedFiles, QStringLiteral("FileUpload_ProtectFiles"));

    mRotateImagesUpload->setObjectName(QStringLiteral("mRotateImagesUpload"));
    mRotateImagesUpload->setToolTip(i18nc("@info:tooltip", "Enabling this setting may cause image quality loss"));
    addCheckBox(mRotateImagesUpload, QStringLiteral("FileUpload_RotateImages"));

    mRestrictFilesToRooms->setObjectName(QStringLiteral("mRestrictFilesToRooms"));
    mRestrictFilesToRooms->setToolTip(i18nc("@info:tooltip", "Restrict the access of files uploaded on rooms to the rooms' members only"));
    addCheckBox(mRestrictFilesToRooms, QStringLiteral("FileUpload_Restrict_to_room_members"));

    mFileUploadsEnabledDirectMessages->setObjectName(QStringLiteral("mFileUploadsEnabledDirectMessages"));
    addCheckBox(mFileUploadsEnabledDirectMessages, QStringLiteral("FileUpload_Enabled_Direct"));

    mAcceptedMediaTypes->setObjectName(QStringLiteral("mAcceptedMediaTypes"));
    mAcceptedMediaTypes->setToolTip(i18nc("@info:tooltip", "Comma-separated list of media types. Leave it blank for accepting all media types."));
    addLineEdit(i18n("Accepted Media Types"), mAcceptedMediaTypes, QStringLiteral("FileUpload_MediaTypeWhiteList"));

    mBlockedMediaTypes->setObjectName(QStringLiteral("mBlockedMediaTypes"));
    mBlockedMediaTypes->setToolTip(i18nc("@info:tooltip", "Comma-separated list of media types. This setting has priority over the Accepted Media Types."));
    addLineEdit(i18n("Accepted Media Types"), mBlockedMediaTypes, QStringLiteral("FileUpload_MediaTypeBlackList"));

    mFileUploadJsonWebTokenSecret->setObjectName(QStringLiteral("mFileUploadJsonWebTokenSecret"));
    mFileUploadJsonWebTokenSecret->setToolTip(
        i18nc("@info:tooltip", "File Upload Json Web Token Secret (Used to be able to access uploaded files without authentication)."));
    addLineEdit(i18n("Accepted Media Types"), mFileUploadJsonWebTokenSecret, QStringLiteral("FileUpload_json_web_token_secret_for_files"));

    mStorageType->setObjectName(QStringLiteral("mStorageType"));
    const QMap<QString, QString> maps = {
        {QStringLiteral("GridFS"), i18n("GridFS")},
        {QStringLiteral("AmazonS3"), i18n("AmazonS3")},
        {QStringLiteral("GoogleCloudStorage"), i18n("GoogleCloudStorage")},
        {QStringLiteral("Webdav"), i18n("WebDAV")},
        {QStringLiteral("FileSystem"), i18n("FileSystem")},
    };
    addComboBox(i18n("Storage Type"), maps, mStorageType, QStringLiteral("FileUpload_Storage_Type"));
}

FileUploadSettingsWidget::~FileUploadSettingsWidget() = default;

void FileUploadSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mFileUploadsEnabled, mapSettings, true);
    initializeWidget(mProtectUploadedFiles, mapSettings, true);
    initializeWidget(mRotateImagesUpload, mapSettings, false);
    initializeWidget(mRestrictFilesToRooms, mapSettings, true);
    initializeWidget(mMaximumFileUploadSize, mapSettings, 104857600);
    initializeWidget(mFileUploadsEnabledDirectMessages, mapSettings, true);
    initializeWidget(mAcceptedMediaTypes, mapSettings, QString());
    initializeWidget(mBlockedMediaTypes, mapSettings, QStringLiteral("image/svg+xml"));
    initializeWidget(mFileUploadJsonWebTokenSecret, mapSettings, QString());
    initializeWidget(mStorageType, mapSettings, QStringLiteral("GridFS"));
}

#include "moc_fileuploadsettingswidget.cpp"
