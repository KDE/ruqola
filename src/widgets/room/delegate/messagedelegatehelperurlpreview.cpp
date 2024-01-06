/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperurlpreview.h"

MessageDelegateHelperUrlPreview::MessageDelegateHelperUrlPreview(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageDelegateHelperUrlPreview::~MessageDelegateHelperUrlPreview() = default;

QTextDocument *MessageDelegateHelperUrlPreview::documentForIndex(const QModelIndex &index) const
{
    return nullptr;
}

QTextDocument *MessageDelegateHelperUrlPreview::documentForIndex(const MessageAttachment &msgAttach) const
{
    return nullptr;
}

QTextDocument *MessageDelegateHelperUrlPreview::documentForIndex(const Block &block) const
{
    return nullptr;
}

void MessageDelegateHelperUrlPreview::draw(const MessageUrl &messageUrl,
                                           QPainter *painter,
                                           QRect attachmentsRect,
                                           const QModelIndex &index,
                                           const QStyleOptionViewItem &option) const
{
    // TODO
}
