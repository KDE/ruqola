/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "lrucache.h"
#include <QItemDelegate>
class QTextDocument;
class RocketChatAccount;
class ApplicationsSettingsDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsDelegate(QObject *parent = nullptr);
    ~ApplicationsSettingsDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    struct Layout {
        // Pixmap
        QPixmap appPixmap;
        QPointF appPixmapPos;

        QString appName;
        QString appShortDescription;
        bool premium = false;
        // TODO
    };

    [[nodiscard]] ApplicationsSettingsDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    [[nodiscard]] QByteArray cacheIdentifier(const QModelIndex &index) const;
    [[nodiscard]] QTextDocument *documentForModelIndex(const QModelIndex &index, int width) const;
    [[nodiscard]] QTextDocument *documentForDelegate(RocketChatAccount *rcAccount, const QByteArray &messageId, const QString &messageStr, int width) const;
    // Cache SizeHint value
    // We need to clear it when we resize widget.
    mutable LRUCache<QByteArray, QSize> mSizeHintCache;
    mutable LRUCache<QByteArray, std::unique_ptr<QTextDocument>> mDocumentCache;
};
