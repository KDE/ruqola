/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "lrucache.h"
#include "misc/messagelistdelegatebase.h"
class QTextDocument;
class RocketChatAccount;
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

    [[nodiscard]] QString selectedText() const;

    [[nodiscard]] bool hasSelection() const;

private:
    enum class Status : uint8_t {
        Unknown,
        Enabled,
        Disabled,
    };
    struct Layout {
        // Text
        QRect textRect;

        // Pixmap
        QPixmap appPixmap;
        QPointF appPixmapPos;

        // Premium
        QString premiumText;
        QRectF premiumRect;
        bool premium = false;

        // Requested
        QString requestedText;
        QRectF requestedRect;
        bool requested = false;

        // Status
        QString statusText;
        QRectF statusRect;
        Status status = Status::Unknown;
    };

    [[nodiscard]] ApplicationsSettingsDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    [[nodiscard]] QByteArray cacheIdentifier(const QModelIndex &index) const;
    [[nodiscard]] QTextDocument *documentForModelIndex(const QModelIndex &index, int width) const override;
    // Cache SizeHint value
    // We need to clear it when we resize widget.
    mutable LRUCache<QByteArray, QSize> mSizeHintCache;
    mutable LRUCache<QByteArray, std::unique_ptr<QTextDocument>> mDocumentCache;
    RocketChatAccount *const mRocketChatAccount;

protected:
    RocketChatAccount *rocketChatAccount(const QModelIndex &index) const override;
};
