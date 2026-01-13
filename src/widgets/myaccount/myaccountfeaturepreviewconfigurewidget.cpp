/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountfeaturepreviewconfigurewidget.h"
#include "rocketchataccount.h"
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
MyAccountFeaturePreviewConfigureWidget::MyAccountFeaturePreviewConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);
}

MyAccountFeaturePreviewConfigureWidget::~MyAccountFeaturePreviewConfigureWidget() = default;

#include "moc_myaccountfeaturepreviewconfigurewidget.cpp"
