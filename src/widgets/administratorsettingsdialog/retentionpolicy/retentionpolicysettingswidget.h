/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RetentionPolicySettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit RetentionPolicySettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~RetentionPolicySettingsWidget() override;

    void initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings) override;

private:
    QCheckBox *const mEnabled;
    QComboBox *const mTimerPrecision;
    QCheckBox *const mUseAdvancedRetentionPolicyConfiguration;
    QCheckBox *const mDontPruneThreads;
    QCheckBox *const mDontPruneDiscussion;
    QCheckBox *const mDontPrunePinnedMessages;
    QCheckBox *const mOnlyDeleteFiles;
    QCheckBox *const mAppliesChannels;
    QSpinBox *const mMaximumMessageChannels;
    QCheckBox *const mAppliesGroups;
    QSpinBox *const mMaximumMessagePrivateGroups;
    QCheckBox *const mAppliesMessages;
    QSpinBox *const mMaximumMessageDirectMessages;
};
