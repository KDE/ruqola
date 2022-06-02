/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "retentionpolicysettingswidget.h"

#include <KLocalizedString>

#include <QCheckBox>
#include <QFormLayout>

RetentionPolicySettingsWidget::RetentionPolicySettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnabled(new QCheckBox(i18n("Enabled"), this))
    , mUseAdvancedRetentionPolicyConfiguration(new QCheckBox(i18n("Use Advanced Retention Policy configuration"), this))
    , mDontPruneThreads(new QCheckBox(i18n("Do not prune Threads"), this))
    , mDontPruneDiscussion(new QCheckBox(i18n("Do not prune Discussion message"), this))
    , mDontPrunePinnedMessages(new QCheckBox(i18n("Do not prune pinned messages"), this))
    , mOnlyDeleteFiles(new QCheckBox(i18n("Only delete files"), this))
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
}

RetentionPolicySettingsWidget::~RetentionPolicySettingsWidget() = default;

void RetentionPolicySettingsWidget::initialize()
{
    // TODO
}
