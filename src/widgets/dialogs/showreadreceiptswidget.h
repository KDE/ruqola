/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class BannedUsersModel;
class RocketChatAccount;
class BannedUsersFilterProxyModel;
class QListView;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowReadReceiptsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowReadReceiptsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowReadReceiptsWidget() override;

    void setRoomId(const QByteArray &roomId);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchReadReceiptsChanged(const QString &str);
    RocketChatAccount *const mCurrentRocketChatAccount;
    QLineEdit *const mShowReadReceiptsLineEdit;
    QListView *const mListReadReceipts;
    BannedUsersModel *const mModel;
    BannedUsersFilterProxyModel *const mReadReceiptsFilterProxyModel;
};
