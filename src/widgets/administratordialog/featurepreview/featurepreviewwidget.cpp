/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "featurepreviewwidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
FeaturePreviewWidget::FeaturePreviewWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mAllowFeaturePreview(new QCheckBox(i18nc("@option:check", "Allow Feature Preview"), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mAllowFeaturePreview->setObjectName(u"mAllowFeaturePreview"_s);
    mainLayout->addWidget(mAllowFeaturePreview);

    mainLayout->addStretch(1);
}

FeaturePreviewWidget::~FeaturePreviewWidget() = default;

void FeaturePreviewWidget::initialize()
{
    // TODO mAllowFeaturePreview->setChecked...
}

#include "moc_featurepreviewwidget.cpp"
