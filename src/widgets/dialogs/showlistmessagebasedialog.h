/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_export.h"
#include <QDialog>

class ShowListMessageBaseWidget;
class ListMessagesFilterProxyModel;
class RocketChatAccount;
class Room;
class LIBRUQOLAWIDGETS_EXPORT ShowListMessageBaseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ShowListMessageBaseDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowListMessageBaseDialog() override;

    void setModel(ListMessagesFilterProxyModel *model);

    void setRoomId(const QString &roomId);
    [[nodiscard]] QString roomId() const;
    void setRoom(Room *room);

Q_SIGNALS:
    void goToMessageRequested(const QString &messageId, const QString &messageDateTimeUtc);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLoadMoreMessages();
    QString mRoomId;
    ShowListMessageBaseWidget *const mShowListMessage;
    RocketChatAccount *const mRocketChatAccount;
};
