/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "messageattachmentdelegatehelperbase.h"

#include <QIcon>
#include <QModelIndex>
#include <QRect>

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageAttachmentDelegateHelperActions : public MessageDelegateHelperBase
{
public:
    explicit MessageAttachmentDelegateHelperActions(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageAttachmentDelegateHelperActions() override;
    void draw(const MessageAttachmentAction &act, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const;
    [[nodiscard]] QSize sizeHint(const MessageAttachmentAction &act, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const;
    [[nodiscard]] virtual bool handleMouseEvent(const MessageAttachmentAction &act,
                                                QMouseEvent *mouseEvent,
                                                QRect attachmentsRect,
                                                const QStyleOptionViewItem &option,
                                                const QModelIndex &index);
};
