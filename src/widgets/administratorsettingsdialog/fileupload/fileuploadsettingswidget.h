/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QSpinBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT FileUploadSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit FileUploadSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~FileUploadSettingsWidget() override;

public:
    void initialize(const QMap<QString, QVariant> &mapSettings) override;

private:
    QCheckBox *const mFileUploadsEnabled;
    QCheckBox *const mProtectUploadedFiles;
    QCheckBox *const mRotateImagesUpload;
    QCheckBox *const mRestrictFilesToRooms;
    QSpinBox *const mMaximumFileUploadSize;
    QCheckBox *const mFileUploadsEnabledDirectMessages;
    QLineEdit *const mAcceptedMediaTypes;
    QLineEdit *const mBlockedMediaTypes;
    QLineEdit *const mFileUploadJsonWebTokenSecret;
    QComboBox *const mStorageType;
};
