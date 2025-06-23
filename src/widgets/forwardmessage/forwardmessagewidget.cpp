/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessagewidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18nc("@label:textbox", "Select at least one channel to forward the message to."), this);
    label->setObjectName(u"label"_s);
    mainLayout->addWidget(label);

    mForwardMessageAddChannelCompletionLineEdit->setObjectName(u"mForwardMessageAddChannelCompletionLineEdit"_s);
    mainLayout->addWidget(mForwardMessageAddChannelCompletionLineEdit);
    connect(mForwardMessageAddChannelCompletionLineEdit,
            &ForwardMessageAddChannelCompletionLineEdit::fowardToChannel,
            this,
            &ForwardMessageWidget::slotForwardToChannel);

    mFlowLayout->setObjectName(u"mFlowLayout"_s);
    mainLayout->addLayout(mFlowLayout);
    mainLayout->addStretch();
}

ForwardMessageWidget::~ForwardMessageWidget()
{
    delete mFlowLayout;
}

void ForwardMessageWidget::slotForwardToChannel(const JoinedChannelCompletionLineEditBase::JoinedChannelCompletionInfo &channelInfo)
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
            mFlowLayout->removeItem(mFlowLayout->itemAt(index));
            mMap.remove(name);
            userWidget->deleteLater();
        }
    }
    Q_EMIT updateOkButton(!mMap.isEmpty());
}

QList<QByteArray> ForwardMessageWidget::channelIdentifiers() const
{
    QList<QByteArray> identifiers;
    QMapIterator<QString, ClickableWidget *> i(mMap);
    identifiers.reserve(mMap.count());
    while (i.hasNext()) {
        i.next();
        identifiers << i.value()->identifier();
    }
    return identifiers;
}

#include "moc_forwardmessagewidget.cpp"
