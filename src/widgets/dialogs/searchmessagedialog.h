/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"

#include <QDialog>
class SearchMessageWidget;
class CommonMessageFilterProxyModel;
class RocketChatAccount;
class Room;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SearchMessageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SearchMessageDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~SearchMessageDialog() override;

    void setRoomId(const QString &roomId);

    void setModel(CommonMessageFilterProxyModel *model);
    void setRoom(Room *room);
Q_SIGNALS:
    void goToMessageRequested(const QString &messageId, const QString &messageDateTimeUtc);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();

    SearchMessageWidget *const mSearchMessageWidget;
};
