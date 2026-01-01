/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userdatadownloadwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>

UserDataDownloadWidget::UserDataDownloadWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mUserDataDownloadEnabled(new QCheckBox(i18nc("@option:check", "Enable User Data Download"), this))
    , mSystemPathExportedFiles(new QLineEdit(this))
    , mSystemPathCompressedFile(new QLineEdit(this))
    , mProcessingFrequency(new QSpinBox(this))
    , mMessageLimitRequest(new QSpinBox(this))
{
    mUserDataDownloadEnabled->setObjectName(u"mUserDataDownloadEnabled"_s);
    addCheckBox(mUserDataDownloadEnabled, u"UserData_EnableDownload"_s);

    mSystemPathExportedFiles->setObjectName(u"mSystemPathExportedFiles"_s);
    addLineEdit(i18n("System Path (Exported Files)"), mSystemPathExportedFiles, u"UserData_FileSystemPath"_s);

    mSystemPathCompressedFile->setObjectName(u"mSystemPathCompressedFile"_s);
    addLineEdit(i18n("System Path (Compressed File)"), mSystemPathCompressedFile, u"UserData_FileSystemZipPath"_s);

    mProcessingFrequency->setObjectName(u"mProcessingFrequency"_s);
    mProcessingFrequency->setMaximum(1000);
    addSpinbox(i18n("Processing Frequency (Minutes)"), mProcessingFrequency, u"UserData_ProcessingFrequency"_s);

    mMessageLimitRequest->setObjectName(u"mMessageLimitRequest"_s);
    mMessageLimitRequest->setMaximum(99999);
    addSpinbox(i18n("Message Limit per Request"), mMessageLimitRequest, u"UserData_MessageLimitPerRequest"_s);
}

UserDataDownloadWidget::~UserDataDownloadWidget() = default;

void UserDataDownloadWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mUserDataDownloadEnabled, mapSettings, true);
    initializeWidget(mSystemPathExportedFiles, mapSettings, QString());
    initializeWidget(mSystemPathCompressedFile, mapSettings, QString());
    initializeWidget(mProcessingFrequency, mapSettings, 2);
    initializeWidget(mMessageLimitRequest, mapSettings, 1000);
}

#include "moc_userdatadownloadwidget.cpp"
