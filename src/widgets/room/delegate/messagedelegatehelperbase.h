/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "delegateutils/textselection.h"
#include "delegateutils/textselectionimpl.h"
#include "libruqolawidgets_private_export.h"
#include <QTextDocument>
#include <lrucache.h>

#include <memory>
class QListView;
class RocketChatAccount;

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperBase : public QObject, public DocumentFactoryInterface
{
public:
    explicit MessageDelegateHelperBase(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    virtual ~MessageDelegateHelperBase();

    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

    void removeMessageCache(const QString &messageId);

    void clearTextDocumentCache();

protected:
    void updateView(const QModelIndex &index);

    QListView *const mListView;
    TextSelectionImpl *const mSelectionImpl;
    RocketChatAccount *mRocketChatAccount = nullptr;
    mutable LRUCache<QString, std::unique_ptr<QTextDocument>, 32> mDocumentCache;
};
