/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperurlpreview.h"

#include "common/delegatepaintutil.h"
#include "messages/messageurl.h"

#include <QPainter>
#include <QStyleOptionViewItem>

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
                                           QRect previewRect,
                                           const QModelIndex &index,
                                           const QStyleOptionViewItem &option) const
{
    const PreviewLayout layout = layoutPreview(messageUrl, option /*, previewsRect.width(), previewsRect.height()*/);
    painter->drawText(previewRect.x(), previewRect.y() + option.fontMetrics.ascent(), layout.title);
    // TODO
}

MessageDelegateHelperUrlPreview::PreviewLayout MessageDelegateHelperUrlPreview::layoutPreview(const MessageUrl &messageUrl,
                                                                                              const QStyleOptionViewItem &option) const
{
    MessageDelegateHelperUrlPreview::PreviewLayout layout;
    layout.title = messageUrl.pageTitle();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);

    layout.description = messageUrl.description();
    layout.imageUrl = messageUrl.imageUrl();
    layout.isShown = messageUrl.showPreview();
    // TODO layout.descriptionSize = documentDescriptionForIndexSize(msgAttach, attachmentsWidth);
    // TODO
    return layout;
}

QTextDocument *MessageDelegateHelperUrlPreview::documentDescriptionForIndex(const Block &block, int width) const
{
    // TODO
    return nullptr;
}

QSize MessageDelegateHelperUrlPreview::documentDescriptionForIndexSize(const Block &block, int width) const
{
    // TODO
    return {};
}

QSize MessageDelegateHelperUrlPreview::sizeHint(const MessageUrl &messageUrl, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    const PreviewLayout layout = layoutPreview(messageUrl, option /*, previewsRect.width(), previewsRect.height()*/);
    int height = layout.titleSize.height() + DelegatePaintUtil::margin();
    int pixmapWidth = 0;
    if (layout.isShown) {
        // pixmapWidth = qMin(layout.pixmap.width(), maxWidth);
        // height += qMin(layout.pixmap.height(), 200) + DelegatePaintUtil::margin();
    }
    int descriptionWidth = 0;
    if (!layout.description.isEmpty()) {
        descriptionWidth = layout.descriptionSize.width();
        height += layout.descriptionSize.height() + DelegatePaintUtil::margin();
    }
    return {qMax(qMax(pixmapWidth, layout.titleSize.width()), descriptionWidth), height};
}

bool MessageDelegateHelperUrlPreview::handleHelpEvent(QHelpEvent *helpEvent,
                                                      QRect messageRect,
                                                      const MessageUrl &messageUrl,
                                                      const QStyleOptionViewItem &option)
{
    // TODO
    return false;
}
