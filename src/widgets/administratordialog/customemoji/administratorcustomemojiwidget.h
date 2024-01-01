/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/searchtreebasewidget.h"
class QWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorCustomEmojiWidget : public SearchTreeBaseWidget
{
    Q_OBJECT
public:
    explicit AdministratorCustomEmojiWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorCustomEmojiWidget() override;

protected:
    void updateLabel() override;
    void slotLoadElements(int offset = -1, int count = -1, const QString &searchName = {}) override;
    void slotCustomContextMenuRequested(const QPoint &pos) override;

private:
    [[nodiscard]] QString displayShowMessage() const;
    void slotModifyCustomEmoji(const QModelIndex &index);
    void slotAddCustomEmoji();
    void slotRemoveCustomEmoji(const QModelIndex &index);
    void slotEmojiRemoved(const QString &emojiId);
};
