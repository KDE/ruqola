/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsversionwidget.h"

#include <QShowEvent>
#include <QTextBrowser>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsVersionWidget::ApplicationsSettingsVersionWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mTextBrowser(new QTextBrowser(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    mTextBrowser->setObjectName("mTextBrowser"_L1);
    mainLayout->addWidget(mTextBrowser);
    mTextBrowser->setOpenExternalLinks(true);
}

ApplicationsSettingsVersionWidget::~ApplicationsSettingsVersionWidget() = default;

void ApplicationsSettingsVersionWidget::showEvent(QShowEvent *event)
{
    if (!event->spontaneous() && !mWasInitialized) {
        mWasInitialized = true;
        initialize();
    }
    QWidget::showEvent(event);
}

void ApplicationsSettingsVersionWidget::initialize()
{
    if (mRocketChatAccount) {
        // TODO
    }
}
