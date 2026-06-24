/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QListView>
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowBannedUsersListView : public QListView
{
    Q_OBJECT
public:
    explicit ShowBannedUsersListView(QWidget *parent = nullptr);
    ~ShowBannedUsersListView() override;

Q_SIGNALS:
    void unbanUser(const QString &userName);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUnbanUser(const QModelIndex &index);
};
