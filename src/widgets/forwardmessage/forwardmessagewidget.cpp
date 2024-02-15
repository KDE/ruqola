/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessagewidget.h"
#include "common/flowlayout.h"
#include "misc/clickablewidget.h"
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
    connect(mForwardMessageAddChannelCompletionLineEdit,
            &ForwardMessageAddChannelCompletionLineEdit::fowardToChannel,
            this,
            &ForwardMessageWidget::slotForwardToChannel);

    mFlowLayout->setObjectName(QStringLiteral("mFlowLayout"));
    mainLayout->addLayout(mFlowLayout);
    mainLayout->addStretch();
}

ForwardMessageWidget::~ForwardMessageWidget()
{
    delete mFlowLayout;
}

void ForwardMessageWidget::slotForwardToChannel(const ForwardMessageAddChannelCompletionLineEdit::ForwardMessageChannelCompletionInfo &channelInfo)
{
    const QString &roomName = channelInfo.name;
    if (mMap.contains(roomName)) {
        return;
    }
    auto clickableWidget = new ClickableWidget(roomName, this);
    clickableWidget->setIdentifier(channelInfo.channelId);
    connect(clickableWidget, &ClickableWidget::removeClickableWidget, this, &ForwardMessageWidget::slotRemoveRoom);
    mFlowLayout->addWidget(clickableWidget);
    mMap.insert(roomName, clickableWidget);
    Q_EMIT updateOkButton(!mMap.isEmpty());
}

void ForwardMessageWidget::slotRemoveRoom(const QString &name)
{
    ClickableWidget *userWidget = mMap.value(name);
    if (userWidget) {
        const int index = mFlowLayout->indexOf(userWidget);
        if (index != -1) {
            delete mFlowLayout->takeAt(index);
            mMap.remove(name);
            delete userWidget;
        }
    }
    Q_EMIT updateOkButton(!mMap.isEmpty());
}

QStringList ForwardMessageWidget::channelIdentifiers() const
{
    // TODO
    return {};
}
#include "moc_forwardmessagewidget.cpp"
