/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"

#include <QDialog>
class ModerationMessagesWidget;
class CommonMessageFilterProxyModel;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ModerationUsersDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ModerationUsersDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ModerationUsersDialog() override;

    void setModel(CommonMessageFilterProxyModel *model);
Q_SIGNALS:
    void goToMessageRequested(const QByteArray &messageId, const QString &messageDateTimeUtc);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();

    ModerationMessagesWidget *const mModerationMessagesWidget;
};
