/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "retentionpolicysettingswidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QSpinBox>

RetentionPolicySettingsWidget::RetentionPolicySettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnabled(new QCheckBox(i18nc("@option:check", "Enabled"), this))
    , mTimerPrecision(new QComboBox(this))
    , mUseAdvancedRetentionPolicyConfiguration(new QCheckBox(i18nc("@option:check", "Use Advanced Retention Policy configuration"), this))
    , mDontPruneThreads(new QCheckBox(i18nc("@option:check", "Do not prune Threads"), this))
    , mDontPruneDiscussion(new QCheckBox(i18nc("@option:check", "Do not prune Discussion message"), this))
    , mDontPrunePinnedMessages(new QCheckBox(i18nc("@option:check", "Do not prune pinned messages"), this))
    , mOnlyDeleteFiles(new QCheckBox(i18nc("@option:check", "Only delete files"), this))
    , mAppliesChannels(new QCheckBox(i18nc("@option:check", "Applies to channels"), this))
    , mMaximumMessageChannels(new QSpinBox(this))
    , mAppliesGroups(new QCheckBox(i18nc("@option:check", "Applies to private groups"), this))
    , mMaximumMessagePrivateGroups(new QSpinBox(this))
    , mAppliesMessages(new QCheckBox(i18nc("@option:check", "Applies to direct messages"), this))
    , mMaximumMessageDirectMessages(new QSpinBox(this))
{
    mEnabled->setObjectName(u"mEnabled"_s);
    addCheckBox(mEnabled, u"RetentionPolicy_Enabled"_s);

    mTimerPrecision->setObjectName(u"mTimerPrecision"_s);
    const QMap<QString, QString> maps = {
        {u"0"_s, i18n("Once every 30 minutes")},
        {u"1"_s, i18n("Once every hour")},
        {u"2"_s, i18n("Once every six hours")},
        {u"3"_s, i18n("Once every day")},
    };
    addComboBox(i18n("Timer Precision"), maps, mTimerPrecision, u"RetentionPolicy_Precision"_s);

    mUseAdvancedRetentionPolicyConfiguration->setObjectName(u"mUseAdvancedRetentionPolicyConfiguration"_s);
    addCheckBox(mUseAdvancedRetentionPolicyConfiguration, u"RetentionPolicy_Advanced_Precision"_s);

    mDontPruneThreads->setObjectName(u"mDontPruneThreads"_s);
    addCheckBox(mDontPruneThreads, u"RetentionPolicy_DoNotPruneThreads"_s);

    mDontPruneDiscussion->setObjectName(u"mDontPruneDiscussion"_s);
    addCheckBox(mDontPruneDiscussion, u"RetentionPolicy_DoNotPruneDiscussion"_s);

    mDontPrunePinnedMessages->setObjectName(u"mDontPrunePinnedMessages"_s);
    addCheckBox(mDontPrunePinnedMessages, u"RetentionPolicy_DoNotPrunePinned"_s);

    mOnlyDeleteFiles->setObjectName(u"mOnlyDeleteFiles"_s);
    mOnlyDeleteFiles->setToolTip(i18nc("@info:tooltip", "Only files will be deleted, the messages themselves will stay in place."));
    addCheckBox(mOnlyDeleteFiles, u"RetentionPolicy_FilesOnly"_s);

    // Retention Channels
    mAppliesChannels->setObjectName(u"mAppliesChannels"_s);
    addCheckBox(mAppliesChannels, u"RetentionPolicy_AppliesToChannels"_s);

    mMaximumMessageChannels->setObjectName(u"mMaximumMessageChannels"_s);
    mMaximumMessageChannels->setToolTip(i18nc("@info:tooltip", "Prune all messages older than this value, in days."));
    mMaximumMessageChannels->setMaximum(9999);
    addSpinbox(i18n("Maximum message age in channels"), mMaximumMessageChannels, u"RetentionPolicy_MaxAge_Channels"_s);

    // Retention Groups
    mAppliesGroups->setObjectName(u"mAppliesGroups"_s);
    addCheckBox(mAppliesGroups, u"RetentionPolicy_AppliesToGroups"_s);

    mMaximumMessagePrivateGroups->setObjectName(u"mMaximumMessagePrivateGroups"_s);
    mMaximumMessagePrivateGroups->setToolTip(i18nc("@info:tooltip", "Prune all messages older than this value, in days."));
    mMaximumMessagePrivateGroups->setMaximum(9999);
    addSpinbox(i18n("Maximum message age in private groups"), mMaximumMessagePrivateGroups, u"RetentionPolicy_MaxAge_Groups"_s);

    // Retention DM
    mAppliesMessages->setObjectName(u"mAppliesMessages"_s);
    addCheckBox(mAppliesMessages, u"RetentionPolicy_AppliesToDMs"_s);

    mMaximumMessageDirectMessages->setObjectName(u"mMaximumMessageDirectMessages"_s);
    mMaximumMessageDirectMessages->setToolTip(i18nc("@info:tooltip", "Prune all messages older than this value, in days."));
    mMaximumMessageDirectMessages->setMaximum(9999);
    addSpinbox(i18n("Maximum message age in direct messages"), mMaximumMessageDirectMessages, u"RetentionPolicy_MaxAge_DMs"_s);
}

RetentionPolicySettingsWidget::~RetentionPolicySettingsWidget() = default;

void RetentionPolicySettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mEnabled, mapSettings, false);
    initializeWidget(mUseAdvancedRetentionPolicyConfiguration, mapSettings, false);
    initializeWidget(mDontPruneThreads, mapSettings, true);
    initializeWidget(mDontPruneDiscussion, mapSettings, true);
    initializeWidget(mDontPrunePinnedMessages, mapSettings, false);
    initializeWidget(mOnlyDeleteFiles, mapSettings, false);
    initializeWidget(mAppliesChannels, mapSettings, false);
    initializeWidget(mAppliesGroups, mapSettings, false);
    initializeWidget(mAppliesMessages, mapSettings, false);
    initializeWidget(mMaximumMessageDirectMessages, mapSettings, 30);
    initializeWidget(mMaximumMessagePrivateGroups, mapSettings, 30);
    initializeWidget(mMaximumMessageChannels, mapSettings, 30);
    initializeWidget(mTimerPrecision, mapSettings, u"0"_s);
}

#include "moc_retentionpolicysettingswidget.cpp"
