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
        QString previewTitle;
        QRect hideShowButtonRect;
        QSize previewTitleSize;
        QSize descriptionSize;
        QSize imageSize;
        bool hasDescription = false;
        bool isShown = true;
    };
    LIBRUQOLAWIDGETS_NO_EXPORT void dump(const PreviewLayout &layout);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT MessageDelegateHelperUrlPreview::PreviewLayout
    layoutPreview(const MessageUrl &messageUrl, const QStyleOptionViewItem &option, int urlsPreviewWidth, int urlsPreviewHeight) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT MessageDelegateHelperBase::DocumentTypeInfo
    convertMessageUrlToDocumentDescriptionInfo(const MessageUrl &messageUrl, int width) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT int
    charPosition(const QTextDocument *doc, const MessageUrl &messageUrl, QRect previewRect, const QPoint &pos, const QStyleOptionViewItem &option);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QPoint adaptMousePosition(const QPoint &pos,
                                                                       const MessageUrl &messageUrl,
                                                                       QRect previewRect,
                                                                       const QStyleOptionViewItem &option);
    LIBRUQOLAWIDGETS_NO_EXPORT void drawDescription(const MessageUrl &messageUrl,
                                                    QRect previewRect,
                                                    QPainter *painter,
                                                    int topPos,
                                                    const QModelIndex &index,
                                                    const QStyleOptionViewItem &option) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QTextDocument *documentForUrlPreview(const MessageUrl &messageUrl) const override;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QPoint relativePos(const QPoint &pos, const PreviewLayout &layout, QRect previewRect) const;

    QPersistentModelIndex mCurrentIndex;
    mutable PixmapCache mPixmapCache;
};
