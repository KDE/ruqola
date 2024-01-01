/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

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

    [[nodiscard]] const QString &searchText() const;
    void setSearchText(const QString &newSearchText);

    [[nodiscard]] QString selectedText() const;

protected:
    [[nodiscard]] bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    [[nodiscard]] bool mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    void slotTextToSpeech(const QModelIndex &index);
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotClearList();
    [[nodiscard]] QString selectedText(const QModelIndex &index) override;
    ServerErrorInfoHistoryDelegate *const mListServerErrorInfosDelegate;
};
