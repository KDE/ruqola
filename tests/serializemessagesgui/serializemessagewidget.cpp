/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serializemessagewidget.h"
#include "rocketchataccount.h"
#include "room/messagelistview.h"
#include <QHBoxLayout>
#include <QPlainTextEdit>

SerializeMessageWidget::SerializeMessageWidget(QWidget *parent)
    : QWidget{parent}
    , mPlainTextEdit(new QPlainTextEdit(this))
    , mMessageListView(new MessageListView(new RocketChatAccount(QStringLiteral("test"), this), MessageListView::Mode::Viewing, this))
    , mMessageModel(new MessagesModel())
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->addWidget(mPlainTextEdit);
    mainLayout->addWidget(mMessageListView);
    mMessageListView->setModel(mMessageModel);
}

SerializeMessageWidget::~SerializeMessageWidget() = default;

void SerializeMessageWidget::addMessage(const Message &msg)
{
    mMessageModel->addMessages({msg});
}

void SerializeMessageWidget::setText(const QString &text)
{
    mPlainTextEdit->setPlainText(text);
}

QString SerializeMessageWidget::text() const
{
    return mPlainTextEdit->toPlainText();
}

#include "moc_serializemessagewidget.cpp"
