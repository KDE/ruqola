/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "retentionpolicysettingswidget.h"

#include <KLocalizedString>

#include <QCheckBox>
#include <QFormLayout>
#include <QSpinBox>

RetentionPolicySettingsWidget::RetentionPolicySettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnabled(new QCheckBox(i18n("Enabled"), this))
    , mUseAdvancedRetentionPolicyConfiguration(new QCheckBox(i18n("Use Advanced Retention Policy configuration"), this))
    , mDontPruneThreads(new QCheckBox(i18n("Do not prune Threads"), this))
    , mDontPruneDiscussion(new QCheckBox(i18n("Do not prune Discussion message"), this))
    , mDontPrunePinnedMessages(new QCheckBox(i18n("Do not prune pinned messages"), this))
    , mOnlyDeleteFiles(new QCheckBox(i18n("Only delete files"), this))
    , mAppliesChannels(new QCheckBox(i18n("Applies to channels"), this))
    , mAppliesGroups(new QCheckBox(i18n("Applies to private groups"), this))
    , mAppliesMessages(new QCheckBox(i18n("Applies to direct messages"), this))
    , mMaximumMessageDirectMessages(new QSpinBox(this))
    , mMaximumMessagePrivateGroups(new QSpinBox(this))
    , mMaximumMessageChannels(new QSpinBox(this))
{
    mEnabled->setObjectName(QStringLiteral("mEnabled"));
    mMainLayout->addWidget(mEnabled);
    connectCheckBox(mEnabled, QStringLiteral("RetentionPolicy_Enabled"));

    mUseAdvancedRetentionPolicyConfiguration->setObjectName(QStringLiteral("mUseAdvancedRetentionPolicyConfiguration"));
    mMainLayout->addWidget(mUseAdvancedRetentionPolicyConfiguration);
    connectCheckBox(mUseAdvancedRetentionPolicyConfiguration, QStringLiteral("RetentionPolicy_Advanced_Precision"));

    mDontPruneThreads->setObjectName(QStringLiteral("mDontPruneThreads"));
    mMainLayout->addWidget(mDontPruneThreads);
    connectCheckBox(mDontPruneThreads, QStringLiteral("RetentionPolicy_DoNotPruneThreads"));

    mDontPruneDiscussion->setObjectName(QStringLiteral("mDontPruneDiscussion"));
    mMainLayout->addWidget(mDontPruneDiscussion);
    connectCheckBox(mDontPruneDiscussion, QStringLiteral("RetentionPolicy_DoNotPruneDiscussion"));

    mDontPrunePinnedMessages->setObjectName(QStringLiteral("mDontPrunePinnedMessages"));
    mMainLayout->addWidget(mDontPrunePinnedMessages);
    connectCheckBox(mDontPrunePinnedMessages, QStringLiteral("RetentionPolicy_DoNotPrunePinned"));

    mOnlyDeleteFiles->setObjectName(QStringLiteral("mOnlyDeleteFiles"));
    mOnlyDeleteFiles->setToolTip(i18n("Only files will be deleted, the messages themselves will stay in place."));
    mMainLayout->addWidget(mOnlyDeleteFiles);
    connectCheckBox(mOnlyDeleteFiles, QStringLiteral("RetentionPolicy_FilesOnly"));

    // Rentention Channels
    mAppliesChannels->setObjectName(QStringLiteral("mAppliesChannels"));
    mMainLayout->addWidget(mAppliesChannels);
    connectCheckBox(mAppliesChannels, QStringLiteral("RetentionPolicy_AppliesToChannels"));

    mMaximumMessageChannels->setObjectName(QStringLiteral("mMaximumMessageChannels"));
    mMaximumMessageChannels->setToolTip(i18n("Prune all messages older than this value, in days."));
    mMaximumMessageChannels->setMaximum(9999);
    addSpinbox(i18n("Maximum message age in channels"), mMaximumMessageChannels, QStringLiteral("RetentionPolicy_MaxAge_Channels"));

    // Rentention Groups
    mAppliesGroups->setObjectName(QStringLiteral("mAppliesGroups"));
    mMainLayout->addWidget(mAppliesGroups);
    connectCheckBox(mAppliesGroups, QStringLiteral("RetentionPolicy_AppliesToGroups"));

    mMaximumMessagePrivateGroups->setObjectName(QStringLiteral("mMaximumMessagePrivateGroups"));
    mMaximumMessagePrivateGroups->setToolTip(i18n("Prune all messages older than this value, in days."));
    mMaximumMessagePrivateGroups->setMaximum(9999);
    addSpinbox(i18n("Maximum message age in private groups"), mMaximumMessagePrivateGroups, QStringLiteral("RetentionPolicy_MaxAge_Groups"));

    // Rentention DM
    mAppliesMessages->setObjectName(QStringLiteral("mAppliesMessages"));
    mMainLayout->addWidget(mAppliesMessages);
    connectCheckBox(mAppliesMessages, QStringLiteral("RetentionPolicy_AppliesToDMs"));

    mMaximumMessageDirectMessages->setObjectName(QStringLiteral("mMaximumMessageDirectMessages"));
    mMaximumMessageDirectMessages->setToolTip(i18n("Prune all messages older than this value, in days."));
    mMaximumMessageDirectMessages->setMaximum(9999);
    addSpinbox(i18n("Maximum message age in direct messages"), mMaximumMessageDirectMessages, QStringLiteral("RetentionPolicy_MaxAge_DMs"));
}

RetentionPolicySettingsWidget::~RetentionPolicySettingsWidget() = default;

void RetentionPolicySettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnabled, mapSettings);
    initializeWidget(mUseAdvancedRetentionPolicyConfiguration, mapSettings);
    initializeWidget(mDontPruneThreads, mapSettings);
    initializeWidget(mDontPruneDiscussion, mapSettings);
    initializeWidget(mDontPrunePinnedMessages, mapSettings);
    initializeWidget(mOnlyDeleteFiles, mapSettings);
    initializeWidget(mAppliesChannels, mapSettings);
    initializeWidget(mAppliesGroups, mapSettings);
    initializeWidget(mAppliesMessages, mapSettings);
    initializeWidget(mMaximumMessageDirectMessages, mapSettings);
    initializeWidget(mMaximumMessagePrivateGroups, mapSettings);
    initializeWidget(mMaximumMessageChannels, mapSettings);
}
