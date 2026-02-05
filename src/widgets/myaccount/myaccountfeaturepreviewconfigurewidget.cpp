/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountfeaturepreviewconfigurewidget.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
MyAccountFeaturePreviewConfigureWidget::MyAccountFeaturePreviewConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    // timestamp by default in RC 8.0.0 => remove it when we will depend against it
    , mTimeStampCheckBox(new QCheckBox(i18nc("@option:check", "Timestamp in messages"), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mTimeStampCheckBox->setObjectName(u"mTimeStampCheckBox"_s);
    mainLayout->addWidget(mTimeStampCheckBox);
    mainLayout->addStretch(1);
}

MyAccountFeaturePreviewConfigureWidget::~MyAccountFeaturePreviewConfigureWidget() = default;

void MyAccountFeaturePreviewConfigureWidget::load()
{
    // TODO
}

void MyAccountFeaturePreviewConfigureWidget::save()
{
    // TODO
}

void MyAccountFeaturePreviewConfigureWidget::initialize()
{
    // TODO
}

#include "moc_myaccountfeaturepreviewconfigurewidget.cpp"
