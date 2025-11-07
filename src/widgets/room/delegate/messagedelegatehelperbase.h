/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

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
    struct DocumentTypeInfo {
        QByteArray identifier;
        QString text;
        int width = -1;
    };
    explicit MessageDelegateHelperBase(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    virtual ~MessageDelegateHelperBase();

    virtual void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

    void removeMessageCache(const QByteArray &messageId) override;

    void clearTextDocumentCache();

    void setSearchText(const QString &newSearchText);
    [[nodiscard]] QString searchText() const;

protected:
    [[nodiscard]] QTextDocument *documentTypeForIndex(const MessageDelegateHelperBase::DocumentTypeInfo &info, bool convertText = true) const;
    [[nodiscard]] QSize documentTypeForIndexSize(const MessageDelegateHelperBase::DocumentTypeInfo &info, bool convertText = true) const;
    void updateView(const QModelIndex &index);
    RocketChatAccount *mRocketChatAccount = nullptr;
    QString mSearchText;
};
Q_DECLARE_TYPEINFO(MessageDelegateHelperBase::DocumentTypeInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageDelegateHelperBase::DocumentTypeInfo &t);
