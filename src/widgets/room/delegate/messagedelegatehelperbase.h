/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "delegateutils/textselection.h"
#include "delegateutils/textselectionimpl.h"
#include "delegateutils/textuibase.h"
#include "libruqolawidgets_private_export.h"

class QListView;
class RocketChatAccount;

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperBase : public QObject, public DocumentFactoryInterface, public TextUiBase
{
public:
    struct DocumentDescriptionInfo {
        QString documentId;
        QString description;
        int width = -1;
    };
    explicit MessageDelegateHelperBase(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    virtual ~MessageDelegateHelperBase();

    virtual void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

    void removeMessageCache(const QString &messageId) override;

    void clearTextDocumentCache();

protected:
    [[nodiscard]] QTextDocument *documentDescriptionForIndex(const MessageDelegateHelperBase::DocumentDescriptionInfo &info) const;
    [[nodiscard]] QSize documentDescriptionForIndexSize(const MessageDelegateHelperBase::DocumentDescriptionInfo &info) const;
    void updateView(const QModelIndex &index);
    RocketChatAccount *mRocketChatAccount = nullptr;
};
Q_DECLARE_TYPEINFO(MessageDelegateHelperBase::DocumentDescriptionInfo, Q_MOVABLE_TYPE);
