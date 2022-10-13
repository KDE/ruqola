/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QLineEdit;
class QCheckBox;
class QSpinBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UserDataDownloadWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit UserDataDownloadWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~UserDataDownloadWidget() override;

    void initialize(const QMap<QString, QVariant> &mapSettings) override;

private:
    QCheckBox *const mUserDataDownloadEnabled;
    QLineEdit *const mSystemPathExportedFiles;
    QLineEdit *const mSystemPathCompressedFile;
    QSpinBox *const mProcessingFrequency;
    QSpinBox *const mMessageLimitRequest;
};
