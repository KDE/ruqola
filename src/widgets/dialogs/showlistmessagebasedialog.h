/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_export.h"
#include "model/listmessagesfilterproxymodel.h"
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

    void setFilterProxyModel(ListMessagesFilterProxyModel *model);
    void setFilteringByType(ListMessagesFilterProxyModel::FilteringByType type);
    void setRoomId(const QByteArray &roomId);
    [[nodiscard]] QByteArray roomId() const;
    void setRoom(Room *room);

    void addMessageLineWidget(QWidget *w);

Q_SIGNALS:
    void goToMessageRequested(const QByteArray &messageId, const QString &messageDateTimeUtc);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLoadMoreMessages();
    QByteArray mRoomId;
    ShowListMessageBaseWidget *const mShowListMessage;
    RocketChatAccount *const mRocketChatAccount;
};
