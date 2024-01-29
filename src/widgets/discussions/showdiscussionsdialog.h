/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ShowDiscussionsWidget;
class DiscussionsFilterProxyModel;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowDiscussionsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ShowDiscussionsDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowDiscussionsDialog() override;

    void setModel(DiscussionsFilterProxyModel *model);
    void setRoomId(const QString &roomId);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLoadMoreDiscussions();
    QString mRoomId;
    ShowDiscussionsWidget *const mShowDiscussionsWidget;
    RocketChatAccount *const mRocketChatAccount;
};
