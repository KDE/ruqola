/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessagewidget.h"
#include "common/flowlayout.h"
#include "forwardmessage/forwardmessageaddchannelcompletionlineedit.h"
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
    connect(mForwardMessageAddChannelCompletionLineEdit, &ForwardMessageAddChannelCompletionLineEdit::newChannel, this, &ForwardMessageWidget::slotAddNewName);

    mFlowLayout->setObjectName(QStringLiteral("mFlowLayout"));
    mainLayout->addLayout(mFlowLayout);
    mainLayout->addStretch();
}

ForwardMessageWidget::~ForwardMessageWidget()
{
    delete mFlowLayout;
}

void ForwardMessageWidget::slotAddNewName(/*const AddTeamRoomCompletionLineEdit::RoomCompletionInfo &info*/)
{
#if 0
    const QString &roomName = info.roomName;
    if (mMap.contains(roomName)) {
        return;
    }
    auto clickableWidget = new ClickableWidget(roomName, this);
    clickableWidget->setIdentifier(info.roomId);
    connect(clickableWidget, &ClickableWidget::removeClickableWidget, this, &ForwardMessageWidget::slotRemoveRoom);
    mFlowLayout->addWidget(clickableWidget);
    mMap.insert(roomName, clickableWidget);
    Q_EMIT userListChanged(!mMap.isEmpty());
#endif
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
    // Q_EMIT userListChanged(!mMap.isEmpty());
}
#include "moc_forwardmessagewidget.cpp"
