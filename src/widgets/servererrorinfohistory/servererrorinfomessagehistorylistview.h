/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/messagelistviewbase.h"
class ServerErrorInfoHistoryDelegate;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ServerErrorInfoMessageHistoryListView : public MessageListViewBase
{
    Q_OBJECT
public:
    explicit ServerErrorInfoMessageHistoryListView(QWidget *parent = nullptr);
    ~ServerErrorInfoMessageHistoryListView() override;

    void clearCache();

    void slotSelectAll(const QModelIndex &index);

    Q_REQUIRED_RESULT const QString &searchText() const;
    void setSearchText(const QString &newSearchText);

    Q_REQUIRED_RESULT QString selectedText() const;

protected:
    Q_REQUIRED_RESULT bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    Q_REQUIRED_RESULT bool mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;

Q_SIGNALS:
    void textToSpeech(const QString &str);

private:
#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
    void slotTextToSpeech(const QModelIndex &index);
#endif
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotClearList();
    void copyMessageToClipboard(const QModelIndex &index);
    ServerErrorInfoHistoryDelegate *const mListServerErrorInfosDelegate;
};
