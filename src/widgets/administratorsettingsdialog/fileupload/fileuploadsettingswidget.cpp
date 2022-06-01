/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fileuploadsettingswidget.h"

#include <QFormLayout>

#include <KLocalizedString>
#include <QCheckBox>
#include <QSpinBox>

FileUploadSettingsWidget::FileUploadSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mFileUploadsEnabled(new QCheckBox(i18n("Enabled File Upload"), this))
    , mProtectUploadedFiles(new QCheckBox(i18n("Protect Uploaded Files"), this))
    , mRotateImagesUpload(new QCheckBox(i18n("Rotate images on upload"), this))
    , mMaximumFileUploadSize(new QSpinBox(this))
{
    mFileUploadsEnabled->setObjectName(QStringLiteral("mFileUploadsEnabled"));
    mMainLayout->addWidget(mFileUploadsEnabled);
    connectCheckBox(mFileUploadsEnabled, QStringLiteral("FileUpload_Enabled"));

    mMaximumFileUploadSize->setObjectName(QStringLiteral("mMaximumFileUploadSize"));
    mMaximumFileUploadSize->setToolTip(i18n("Set it to -1 to remove the file size limitation."));
    addSpinbox(i18n("Maximum File Upload Size (in bytes)"), mMaximumFileUploadSize, QStringLiteral("FileUpload_MaxFileSize"));

    mProtectUploadedFiles->setObjectName(QStringLiteral("mProtectUploadedFiles"));
    mProtectUploadedFiles->setToolTip(i18n("Only authenticated users will have access"));
    mMainLayout->addWidget(mProtectUploadedFiles);
    connectCheckBox(mProtectUploadedFiles, QStringLiteral("FileUpload_ProtectFiles"));

    mRotateImagesUpload->setObjectName(QStringLiteral("mRotateImagesUpload"));
    mRotateImagesUpload->setToolTip(i18n("Enabling this setting may cause image quality loss"));
    mMainLayout->addWidget(mRotateImagesUpload);
    connectCheckBox(mRotateImagesUpload, QStringLiteral("FileUpload_RotateImages"));
}

FileUploadSettingsWidget::~FileUploadSettingsWidget() = default;

void FileUploadSettingsWidget::initialize()
{
    // TODO
}
