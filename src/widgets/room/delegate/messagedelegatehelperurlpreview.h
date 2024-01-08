/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "messagedelegatehelperbase.h"
class QStyleOptionViewItem;
class MessageUrl;
class QHelpEvent;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperUrlPreview : public MessageDelegateHelperBase
{
public:
    explicit MessageDelegateHelperUrlPreview(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageDelegateHelperUrlPreview() override;
    [[nodiscard]] QTextDocument *documentForIndex(const QModelIndex &index) const override;
    [[nodiscard]] QTextDocument *documentForIndex(const MessageAttachment &msgAttach) const override;
    [[nodiscard]] QTextDocument *documentForIndex(const Block &block) const override;

    void draw(const MessageUrl &messageUrl, QPainter *painter, QRect attachmentsRect, const QModelIndex &index, const QStyleOptionViewItem &option) const;

    [[nodiscard]] QSize sizeHint(const MessageUrl &messageUrl, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const;

    [[nodiscard]] bool handleHelpEvent(QHelpEvent *helpEvent, QRect messageRect, const MessageUrl &messageUrl, const QStyleOptionViewItem &option);

private:
    Q_DISABLE_COPY(MessageDelegateHelperUrlPreview)
    struct PreviewLayout {
        QString title;
        QString description;
        QString imageUrl;
        QSize titleSize;
        QSize imageSize;
        QSize descriptionSize;
        QRect hideShowButtonRect;
        bool isShown = true;
    };
    [[nodiscard]] MessageDelegateHelperUrlPreview::PreviewLayout layoutPreview(const MessageUrl &messageUrl, const QStyleOptionViewItem &option) const;
    [[nodiscard]] QTextDocument *documentDescriptionForIndex(const MessageUrl &messageUrl, int width) const;
    [[nodiscard]] QSize documentDescriptionForIndexSize(const MessageUrl &messageUrl, int width) const;
};
