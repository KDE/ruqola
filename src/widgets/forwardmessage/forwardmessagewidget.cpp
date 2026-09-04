/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessagewidget.h"

#include "misc/clickablewidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>
#include <TextAddonsWidgets/TextAddonsWidgetFlowLayout>

using namespace Qt::Literals::StringLiterals;
ForwardMessageWidget::ForwardMessageWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mForwardMessageAddChannelCompletionLineEdit(new ForwardMessageAddChannelCompletionLineEdit(account, this))
    , mFlowLayout(new TextAddonsWidgets::TextAddonsWidgetFlowLayout)
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
            &ForwardMessageAddChannelCompletionLineEdit::forwardToChannel,
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
    const QByteArray &channelId = channelInfo.channelId;
    if (mMap.contains(channelId)) {
        return;
    }
    auto clickableWidget = new ClickableWidget(channelInfo.name, this);
    clickableWidget->setIdentifier(channelId);
    connect(clickableWidget, &ClickableWidget::removeClickableWidget, this, &ForwardMessageWidget::slotRemoveRoom);
    mFlowLayout->addWidget(clickableWidget);
    mMap.insert(channelId, clickableWidget);
    Q_EMIT updateOkButton(!mMap.isEmpty());
}

void ForwardMessageWidget::slotRemoveRoom(const QByteArray &channelId)
{
    if (ClickableWidget *const roomWidget = mMap.take(channelId)) {
        if (const int index = mFlowLayout->indexOf(roomWidget); index != -1) {
            delete mFlowLayout->takeAt(index);
        }
        roomWidget->deleteLater();
        Q_EMIT updateOkButton(!mMap.isEmpty());
    }
}

QList<QByteArray> ForwardMessageWidget::channelIdentifiers() const
{
    QList<QByteArray> identifiers;
    identifiers.reserve(mMap.count());
    for (const auto &[key, value] : mMap.asKeyValueRange()) {
        identifiers << value->identifier();
    }
    return identifiers;
}

#include "moc_forwardmessagewidget.cpp"
