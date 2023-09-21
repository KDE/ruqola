/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "delegateutils/textselection.h"
#include "libruqolawidgets_private_export.h"
#include "lrucache.h"
#include <QItemDelegate>
class QTextDocument;
class TextSelectionImpl;
class RocketChatAccount;
class QListView;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageListDelegateBase : public QItemDelegate, public DocumentFactoryInterface
{
    Q_OBJECT
public:
    explicit MessageListDelegateBase(QListView *view, QObject *parent = nullptr);
    ~MessageListDelegateBase() override;

    void clearCache();
    [[nodiscard]] bool maybeStartDrag(QMouseEvent *mouseEvent, QRect messageRect, const QStyleOptionViewItem &option, const QModelIndex &index);

    [[nodiscard]] bool handleMouseEvent(QMouseEvent *mouseEvent, QRect messageRect, const QStyleOptionViewItem &option, const QModelIndex &index);
    void selectAll(const QStyleOptionViewItem &option, const QModelIndex &index);

    [[nodiscard]] const QString &searchText() const;
    void setSearchText(const QString &newSearchText);

    [[nodiscard]] QString selectedText() const;

    [[nodiscard]] bool hasSelection() const;

Q_SIGNALS:
    void updateView(const QModelIndex &index);

protected:
    [[nodiscard]] QTextDocument *documentForIndex(const QModelIndex &index) const override;
    [[nodiscard]] QTextDocument *documentForIndex(const MessageAttachment &msgAttach) const override;
    [[nodiscard]] QTextDocument *documentForIndex(const Block &block) const override;
    [[nodiscard]] QSize textSizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option, qreal *pBaseLine) const;

    virtual QTextDocument *documentForModelIndex(const QModelIndex &index, int width) const = 0;
    [[nodiscard]] virtual RocketChatAccount *rocketChatAccount(const QModelIndex &index) const = 0;

    TextSelectionImpl *const mTextSelectionImpl;
    mutable LRUCache<QString, std::unique_ptr<QTextDocument>, 32> mDocumentCache;

    QString mSearchText;
    QListView *const mListView;
};
