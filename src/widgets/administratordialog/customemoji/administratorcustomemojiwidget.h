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
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString displayShowMessage() const;
    LIBRUQOLAWIDGETS_NO_EXPORT void slotModifyCustomEmoji(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAddCustomEmoji();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemoveCustomEmoji(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotEmojiRemoved(const QString &emojiId);
};
