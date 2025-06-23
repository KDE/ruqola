/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fileuploadsettingswidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mFileUploadsEnabled->setObjectName(u"mFileUploadsEnabled"_s);
    addCheckBox(mFileUploadsEnabled, u"FileUpload_Enabled"_s);

    mMaximumFileUploadSize->setObjectName(u"mMaximumFileUploadSize"_s);
    mMaximumFileUploadSize->setToolTip(i18nc("@info:tooltip", "Set it to -1 to remove the file size limitation."));
    mMaximumFileUploadSize->setMaximum(1215752191);
    addSpinbox(i18n("Maximum File Upload Size (in bytes)"), mMaximumFileUploadSize, u"FileUpload_MaxFileSize"_s);

    mProtectUploadedFiles->setObjectName(u"mProtectUploadedFiles"_s);
    mProtectUploadedFiles->setToolTip(i18nc("@info:tooltip", "Only authenticated users will have access"));
    addCheckBox(mProtectUploadedFiles, u"FileUpload_ProtectFiles"_s);

    mRotateImagesUpload->setObjectName(u"mRotateImagesUpload"_s);
    mRotateImagesUpload->setToolTip(i18nc("@info:tooltip", "Enabling this setting may cause image quality loss"));
    addCheckBox(mRotateImagesUpload, u"FileUpload_RotateImages"_s);

    mRestrictFilesToRooms->setObjectName(u"mRestrictFilesToRooms"_s);
    mRestrictFilesToRooms->setToolTip(i18nc("@info:tooltip", "Restrict the access of files uploaded on rooms to the rooms' members only"));
    addCheckBox(mRestrictFilesToRooms, u"FileUpload_Restrict_to_room_members"_s);

    mFileUploadsEnabledDirectMessages->setObjectName(u"mFileUploadsEnabledDirectMessages"_s);
    addCheckBox(mFileUploadsEnabledDirectMessages, u"FileUpload_Enabled_Direct"_s);

    mAcceptedMediaTypes->setObjectName(u"mAcceptedMediaTypes"_s);
    mAcceptedMediaTypes->setToolTip(i18nc("@info:tooltip", "Comma-separated list of media types. Leave it blank for accepting all media types."));
    addLineEdit(i18n("Accepted Media Types"), mAcceptedMediaTypes, u"FileUpload_MediaTypeWhiteList"_s);

    mBlockedMediaTypes->setObjectName(u"mBlockedMediaTypes"_s);
    mBlockedMediaTypes->setToolTip(i18nc("@info:tooltip", "Comma-separated list of media types. This setting has priority over the Accepted Media Types."));
    addLineEdit(i18n("Accepted Media Types"), mBlockedMediaTypes, u"FileUpload_MediaTypeBlackList"_s);

    mFileUploadJsonWebTokenSecret->setObjectName(u"mFileUploadJsonWebTokenSecret"_s);
    mFileUploadJsonWebTokenSecret->setToolTip(
        i18nc("@info:tooltip", "File Upload Json Web Token Secret (Used to be able to access uploaded files without authentication)."));
    addLineEdit(i18n("Accepted Media Types"), mFileUploadJsonWebTokenSecret, u"FileUpload_json_web_token_secret_for_files"_s);

    mStorageType->setObjectName(u"mStorageType"_s);
    const QMap<QString, QString> maps = {
        {u"GridFS"_s, i18n("GridFS")},
        {u"AmazonS3"_s, i18n("AmazonS3")},
        {u"GoogleCloudStorage"_s, i18n("GoogleCloudStorage")},
        {u"Webdav"_s, i18n("WebDAV")},
        {u"FileSystem"_s, i18n("FileSystem")},
    };
    addComboBox(i18n("Storage Type"), maps, mStorageType, u"FileUpload_Storage_Type"_s);
}

FileUploadSettingsWidget::~FileUploadSettingsWidget() = default;

void FileUploadSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mFileUploadsEnabled, mapSettings, true);
    initializeWidget(mProtectUploadedFiles, mapSettings, true);
    initializeWidget(mRotateImagesUpload, mapSettings, false);
    initializeWidget(mRestrictFilesToRooms, mapSettings, true);
    initializeWidget(mMaximumFileUploadSize, mapSettings, 104857600);
    initializeWidget(mFileUploadsEnabledDirectMessages, mapSettings, true);
    initializeWidget(mAcceptedMediaTypes, mapSettings, QString());
    initializeWidget(mBlockedMediaTypes, mapSettings, u"image/svg+xml"_s);
    initializeWidget(mFileUploadJsonWebTokenSecret, mapSettings, QString());
    initializeWidget(mStorageType, mapSettings, u"GridFS"_s);
}

#include "moc_fileuploadsettingswidget.cpp"
