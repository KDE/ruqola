/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "messages/message.h"
#include <QWidget>
class QPlainTextEdit;
class MessageListView;
class MessagesModel;
class SerializeMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SerializeMessageWidget(QWidget *parent = nullptr);
    ~SerializeMessageWidget() override;
    void setReadOnly(bool b);

    void setText(const QString &text);
    [[nodiscard]] QString text() const;
    void addMessage(const Message &msg);

private:
    QPlainTextEdit *const mPlainTextEdit;
    MessageListView *const mMessageListView;
    MessagesModel *const mMessageModel;
};
