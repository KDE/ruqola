/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QSpinBox;
class QFormLayout;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit MessageSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MessageSettingsWidget() override;

    void initialize() override;

private:
    QCheckBox *const mAllowMessageEditing;
    QCheckBox *const mAllowMessageDeleting;
    QCheckBox *const mShowEditedStatus;
    QCheckBox *const mShowDeletedStatus;
    QCheckBox *const mAllowMessagePinning;
    QCheckBox *const mAllowMessageSnippeting;
    QCheckBox *const mAllowConvertingLongMessageAttachment;
    QCheckBox *const mVideoRecorderEnabled;
    QSpinBox *const mBlockMessageEditingAfterMinutes;
};
