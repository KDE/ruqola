/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_export.h"
#include <QDialog>

class ShowListMessageBaseWidget;
class ListMessagesModelFilterProxyModel;
class RocketChatAccount;
class Room;
class LIBRUQOLAWIDGETS_EXPORT ShowListMessageBaseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ShowListMessageBaseDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowListMessageBaseDialog() override;

    void setModel(ListMessagesModelFilterProxyModel *model);

    void setRoomId(const QString &roomId);
    Q_REQUIRED_RESULT QString roomId() const;
    void setRoom(Room *room);

Q_SIGNALS:
    void goToMessageRequested(const QString &messageId, const QString &messageDateTimeUtc);

private:
    void slotLoadMoreMessages();
    QString mRoomId;
    ShowListMessageBaseWidget *const mShowListMessage;
    RocketChatAccount *const mRocketChatAccount;
};
