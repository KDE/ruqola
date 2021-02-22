/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "channelinfoprunewidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>

ChannelInfoPruneWidget::ChannelInfoPruneWidget(QWidget *parent)
    : QWidget(parent)
    , mExcludePinnedMessages(new QCheckBox(i18n("Exclude pinned messages"), this))
    , mPruneFileOnlyKeepMessages(new QCheckBox(i18n("Prune files only, keep messages"), this))
    , mAutomaticPruneOldMessages(new QCheckBox(i18n("Automatically prune old messages"), this))
    , mOverrideGlobalRetentionPolicy(new QCheckBox(i18n("Override global retention policy"), this))
    , mMaximumAgeInDay(new QSpinBox(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mAutomaticPruneOldMessages->setObjectName(QStringLiteral("mAutomaticPruneOldMessages"));
    mainLayout->addWidget(mAutomaticPruneOldMessages);
    mOverrideGlobalRetentionPolicy->setObjectName(QStringLiteral("mOverrideGlobalRetentionPolicy"));
    mainLayout->addWidget(mOverrideGlobalRetentionPolicy);
    mExcludePinnedMessages->setObjectName(QStringLiteral("mExcludePinnedMessages"));
    mainLayout->addWidget(mExcludePinnedMessages);
    mPruneFileOnlyKeepMessages->setObjectName(QStringLiteral("mPruneFileOnlyKeepMessages"));
    mainLayout->addWidget(mPruneFileOnlyKeepMessages);

    auto label = new QLabel(i18n("Maximum message age in days (default: 300)"), this);
    label->setObjectName(QStringLiteral("label"));
    mMaximumAgeInDay->setObjectName(QStringLiteral("mMaximumAgeInDay"));
    mainLayout->addWidget(label);
    mainLayout->addWidget(mMaximumAgeInDay);
}

ChannelInfoPruneWidget::~ChannelInfoPruneWidget()
{
}
