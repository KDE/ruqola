/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "messagedelegatehelperbase.h"
#include <QModelIndex>
#include <QRect>
#include <QTextDocument>

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageAttachmentDelegateHelperText : public MessageDelegateHelperBase
{
public:
    ~MessageAttachmentDelegateHelperText() override;
    void
    draw(const MessageAttachment &msgAttach, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const override;
    QSize sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const override;
    bool handleMouseEvent(const MessageAttachment &msgAttach,
                          QMouseEvent *mouseEvent,
                          QRect attachmentsRect,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) override;

private:
    struct TextLayout {
        QString title;
        QSizeF titleSize;
        QRect hideShowButtonRect;
        QSize textSize;
        QFont textFont;
        bool isShown = true;
    };
    Q_REQUIRED_RESULT TextLayout layoutText(const MessageAttachment &msgAttach,
                                            const QStyleOptionViewItem &option,
                                            int attachmentsWidth,
                                            int attachmentsHeight) const;
    Q_REQUIRED_RESULT QTextDocument *documentForIndex(const MessageAttachment &msgAttach, int width) const;
};

