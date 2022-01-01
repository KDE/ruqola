/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"

#include <QDialog>
class SearchMessageWidget;
class SearchMessageFilterProxyModel;
class RocketChatAccount;
class Room;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SearchMessageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SearchMessageDialog(QWidget *parent = nullptr);
    ~SearchMessageDialog() override;

    void setRoomId(const QString &roomId);

    void setModel(SearchMessageFilterProxyModel *model);
    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);
    void setRoom(Room *room);
Q_SIGNALS:
    void goToMessageRequested(const QString &messageId, const QString &messageDateTimeUtc);

private:
    void readConfig();
    void writeConfig();

    SearchMessageWidget *const mSearchMessageWidget;
};

