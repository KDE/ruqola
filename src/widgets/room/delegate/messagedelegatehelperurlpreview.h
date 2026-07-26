/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "messagedelegatehelperbase.h"
#include "misc/pixmapcache.h"
#include <QPixmap>
class QStyleOptionViewItem;
class MessageUrl;
class QMouseEvent;
class QHelpEvent;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperUrlPreview : public MessageDelegateHelperBase
{
public:
    explicit MessageDelegateHelperUrlPreview(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageDelegateHelperUrlPreview() override;

    void draw(const MessageUrl &messageUrl, QPainter *painter, QRect previewRect, const QModelIndex &index, const QStyleOptionViewItem &option) const;

    [[nodiscard]] QSize sizeHint(const MessageUrl &messageUrl, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const;

    [[nodiscard]] bool handleHelpEvent(QHelpEvent *helpEvent, QRect previewRect, const MessageUrl &messageUrl, const QStyleOptionViewItem &option);

    [[nodiscard]] bool
    handleMouseEvent(const MessageUrl &messageUrl, QMouseEvent *mouseEvent, QRect previewRect, const QStyleOptionViewItem &option, const QModelIndex &index);

    [[nodiscard]] QString urlAt(const QStyleOptionViewItem &option, const MessageUrl &messageUrl, QRect previewsRect, QPoint pos);

    [[nodiscard]] bool
    maybeStartDrag(const MessageUrl &messageUrl, QMouseEvent *mouseEvent, QRect previewsRect, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    struct PreviewLayout {
        QPixmap pixmap;
        QString imageUrl;
        // Elided page title, drawn as clickable text when the preview is collapsed.
        QString collapsedTitle;
        QRect hideShowButtonRect;
        QRect collapsedTitleRect;
        QSize collapsedTitleSize;
        QSize descriptionSize;
        // Thumbnail size in device pixels (the pixmap carries the device pixel ratio).
        QSize imageSize;
        // Logical-pixel geometry of the compact card. textLeftOffset is where the
        // description document starts (to the right of the thumbnail); docWidth is
        // the text width it is laid out at; contentWidth/Height is the whole card.
        // sizeHint() reports contentWidth/Height as the slot size; draw() re-derives the
        // rest from the rect it is handed, which is that slot (see draw()).
        int textLeftOffset = 0;
        int docWidth = 0;
        int contentWidth = 0;
        int contentHeight = 0;
        bool isShown = true;
    };
    LIBRUQOLAWIDGETS_NO_EXPORT void dump(const PreviewLayout &layout);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT MessageDelegateHelperUrlPreview::PreviewLayout
    layoutPreview(const MessageUrl &messageUrl, const QStyleOptionViewItem &option, int urlsPreviewWidth, int urlsPreviewHeight) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT MessageDelegateHelperBase::DocumentTypeInfo
    convertMessageUrlToDocumentDescriptionInfo(const MessageUrl &messageUrl, int width) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT int charPosition(const QTextDocument *doc, const PreviewLayout &layout, QRect previewRect, const QPoint &pos);
    LIBRUQOLAWIDGETS_NO_EXPORT void drawDescription(const MessageUrl &messageUrl,
                                                    QRect previewRect,
                                                    QPainter *painter,
                                                    const QModelIndex &index,
                                                    const QStyleOptionViewItem &option,
                                                    const PreviewLayout &layout) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QTextDocument *documentForUrlPreview(const MessageUrl &messageUrl) const override;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QPoint relativePos(const QPoint &pos, const PreviewLayout &layout, QRect previewRect) const;

    QPersistentModelIndex mCurrentIndex;
    mutable PixmapCache mPixmapCache;
};
