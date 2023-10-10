/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/messagelistviewbase.h"
class ModerationMessageDelegate;

class LIBRUQOLAWIDGETS_TESTS_EXPORT ModerationMessageListView : public MessageListViewBase
{
    Q_OBJECT
public:
    explicit ModerationMessageListView(QWidget *parent = nullptr);
    ~ModerationMessageListView() override;

    void clearCache();

    void slotSelectAll(const QModelIndex &index);

    [[nodiscard]] const QString &searchText() const;
    void setSearchText(const QString &newSearchText);

    [[nodiscard]] QString selectedText() const;

protected:
    [[nodiscard]] bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    [[nodiscard]] bool mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;

Q_SIGNALS:
    void showMessage(const QModelIndex &index);

private:
    [[nodiscard]] QString selectedText(const QModelIndex &index) override;
    void slotTextToSpeech(const QModelIndex &index);
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotClearList();
    ModerationMessageDelegate *const mListNotificationsDelegate;
};
