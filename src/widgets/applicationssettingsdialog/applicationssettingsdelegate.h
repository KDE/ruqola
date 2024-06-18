/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "lrucache.h"
#include "misc/messagelistdelegatebase.h"
class QTextDocument;
class RocketChatAccount;
class QTreeView;
class ApplicationsSettingsDelegate : public MessageListDelegateBase
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsDelegate(RocketChatAccount *account, QAbstractItemView *view, QObject *parent = nullptr);
    ~ApplicationsSettingsDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    [[nodiscard]] bool mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

    [[nodiscard]] bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    struct Layout {
        QRect textRect;
        // Pixmap
        QPixmap appPixmap;
        QPointF appPixmapPos;

        bool premium = false;
        // TODO
    };

    [[nodiscard]] ApplicationsSettingsDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    [[nodiscard]] QByteArray cacheIdentifier(const QModelIndex &index) const;
    [[nodiscard]] QTextDocument *documentForModelIndex(const QModelIndex &index, int width) const override;
    // Cache SizeHint value
    // We need to clear it when we resize widget.
    mutable LRUCache<QByteArray, QSize> mSizeHintCache;
    mutable LRUCache<QByteArray, std::unique_ptr<QTextDocument>> mDocumentCache;
    RocketChatAccount *const mRocketChatAccount;

    // MessageListDelegateBase interface
protected:
    RocketChatAccount *rocketChatAccount(const QModelIndex &index) const override;
};
