/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessagewidget.h"
#include "common/flowlayout.h"
#include "forwardmessage/forwardmessageaddchannelcompletionlineedit.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

ForwardMessageWidget::ForwardMessageWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mForwardMessageAddChannelCompletionLineEdit(new ForwardMessageAddChannelCompletionLineEdit(account, this))
    , mFlowLayout(new FlowLayout)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    auto label = new QLabel(i18n("Select at least one channel to forward the message to."), this);
    label->setObjectName(QStringLiteral("label"));
    mainLayout->addWidget(label);

    mForwardMessageAddChannelCompletionLineEdit->setObjectName(QStringLiteral("mForwardMessageAddChannelCompletionLineEdit"));
    mainLayout->addWidget(mForwardMessageAddChannelCompletionLineEdit);
    mFlowLayout->setObjectName(QStringLiteral("mFlowLayout"));
    mainLayout->addLayout(mFlowLayout);
    mainLayout->addStretch();
}

ForwardMessageWidget::~ForwardMessageWidget()
{
    delete mFlowLayout;
}

#include "moc_forwardmessagewidget.cpp"
