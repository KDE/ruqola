/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fileuploadsettingswidget.h"

#include <QFormLayout>

#include <KLocalizedString>
#include <QCheckBox>

FileUploadSettingsWidget::FileUploadSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mFileUploadsEnabled(new QCheckBox(i18n("Enabled File Upload"), this))
    , mProtectUploadedFiles(new QCheckBox(i18n("Protect Uploaded Files"), this))
    , mRotateImagesUpload(new QCheckBox(i18n("Rotate images on upload"), this))
{
    auto mainLayout = new QFormLayout(mCurrentWidget);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mFileUploadsEnabled->setObjectName(QStringLiteral("mFileUploadsEnabled"));
    mainLayout->addWidget(mFileUploadsEnabled);
    connectCheckBox(mFileUploadsEnabled, QStringLiteral("FileUpload_Enabled"));

    mProtectUploadedFiles->setObjectName(QStringLiteral("mProtectUploadedFiles"));
    mProtectUploadedFiles->setToolTip(i18n("Only authenticated users will have access"));
    mainLayout->addWidget(mProtectUploadedFiles);
    connectCheckBox(mProtectUploadedFiles, QStringLiteral("FileUpload_ProtectFiles"));

    mRotateImagesUpload->setObjectName(QStringLiteral("mRotateImagesUpload"));
    mRotateImagesUpload->setToolTip(i18n("Enabling this setting may cause image quality loss"));
    mainLayout->addWidget(mRotateImagesUpload);
    connectCheckBox(mRotateImagesUpload, QStringLiteral("FileUpload_RotateImages"));
}

FileUploadSettingsWidget::~FileUploadSettingsWidget() = default;

void FileUploadSettingsWidget::initialize()
{
    // TODO
}
