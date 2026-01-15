/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "featurepreviewwidget.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
FeaturePreviewWidget::FeaturePreviewWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mAllowFeaturePreview(new QCheckBox(i18nc("@option:check", "Allow Feature Preview"), this))
    , mQuickReactions(new QCheckBox(i18nc("@option:check", "Quick Reactions"), this))
    , mTimestampInMessages(new QCheckBox(i18nc("@option:check", "Timestamp in Messages"), this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mAllowFeaturePreview->setObjectName(u"mAllowFeaturePreview"_s);
    mainLayout->addWidget(mAllowFeaturePreview);

    mQuickReactions->setObjectName(u"mQuickReactions"_s);
    mainLayout->addWidget(mQuickReactions);

    mTimestampInMessages->setObjectName(u"mTimestampInMessages"_s);
    mainLayout->addWidget(mTimestampInMessages);
    // Move in stable in RC 8.0.0
    if (account && account->hasAtLeastVersion(8, 0, 0)) {
        mQuickReactions->hide();
        mTimestampInMessages->hide();
    }

    mainLayout->addStretch(1);
}

FeaturePreviewWidget::~FeaturePreviewWidget() = default;

void FeaturePreviewWidget::initialize()
{
    // TODO mAllowFeaturePreview->setChecked...
}

#include "moc_featurepreviewwidget.cpp"
