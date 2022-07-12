/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "delegateutils/textselection.h"
#include "libruqolawidgets_private_export.h"
#include "lrucache.h"
#include <QItemDelegate>
#include <QTextDocument>
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
    Q_REQUIRED_RESULT bool maybeStartDrag(QMouseEvent *mouseEvent, QRect messageRect, const QStyleOptionViewItem &option, const QModelIndex &index);

    Q_REQUIRED_RESULT bool handleMouseEvent(QMouseEvent *mouseEvent, QRect messageRect, const QStyleOptionViewItem &option, const QModelIndex &index);
    void selectAll(const QStyleOptionViewItem &option, const QModelIndex &index);

    Q_REQUIRED_RESULT const QString &searchText() const;
    void setSearchText(const QString &newSearchText);

Q_SIGNALS:
    void updateView(const QModelIndex &index);

protected:
    Q_REQUIRED_RESULT QTextDocument *documentForIndex(const QModelIndex &index) const override;
    Q_REQUIRED_RESULT QTextDocument *documentForIndex(const MessageAttachment &msgAttach) const override;
    Q_REQUIRED_RESULT QSize textSizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option, qreal *pBaseLine) const;

    virtual QTextDocument *documentForModelIndex(const QModelIndex &index, int width) const = 0;
    virtual RocketChatAccount *rocketChatAccount(const QModelIndex &index) const = 0;

    TextSelectionImpl *const mTextSelectionImpl;
    mutable LRUCache<QString, std::unique_ptr<QTextDocument>, 32> mDocumentCache;

    QString mSearchText;
    QListView *const mListView;
};
