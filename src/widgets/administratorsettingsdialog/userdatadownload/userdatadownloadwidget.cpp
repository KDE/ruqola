/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userdatadownloadwidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>

UserDataDownloadWidget::UserDataDownloadWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mUserDataDownloadEnabled(new QCheckBox(i18n("Enable User Data Download"), this))
    , mSystemPathExportedFiles(new QLineEdit(this))
    , mSystemPathCompressedFile(new QLineEdit(this))
    , mProcessingFrequency(new QSpinBox(this))
    , mMessageLimitRequest(new QSpinBox(this))
{
    mUserDataDownloadEnabled->setObjectName(QStringLiteral("mUserDataDownloadEnabled"));
    mMainLayout->addWidget(mUserDataDownloadEnabled);
    connectCheckBox(mUserDataDownloadEnabled, QStringLiteral("UserData_EnableDownload"));

    mSystemPathExportedFiles->setObjectName(QStringLiteral("mSystemPathExportedFiles"));
    addLineEdit(i18n("System Path (Exported Files)"), mSystemPathExportedFiles, QStringLiteral("UserData_FileSystemPath"));

    mSystemPathCompressedFile->setObjectName(QStringLiteral("mSystemPathCompressedFile"));
    addLineEdit(i18n("System Path (Compressed File)"), mSystemPathCompressedFile, QStringLiteral("UserData_FileSystemZipPath"));

    mProcessingFrequency->setObjectName(QStringLiteral("mProcessingFrequency"));
    mProcessingFrequency->setMaximum(1000);
    addSpinbox(i18n("Processing Frequency (Minutes)"), mProcessingFrequency, QStringLiteral("UserData_ProcessingFrequency"));

    mMessageLimitRequest->setObjectName(QStringLiteral("mMessageLimitRequest"));
    mMessageLimitRequest->setMaximum(99999);
    addSpinbox(i18n("Message Limit per Request"), mMessageLimitRequest, QStringLiteral("UserData_MessageLimitPerRequest"));
}

UserDataDownloadWidget::~UserDataDownloadWidget() = default;

void UserDataDownloadWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mUserDataDownloadEnabled, mapSettings, true);
    initializeWidget(mSystemPathExportedFiles, mapSettings, QString());
    initializeWidget(mSystemPathCompressedFile, mapSettings, QString());
    initializeWidget(mProcessingFrequency, mapSettings, 2);
    initializeWidget(mMessageLimitRequest, mapSettings, 1000);
}

#include "moc_userdatadownloadwidget.cpp"
