/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class RocketChatAccount;
class ShowReadReceiptsWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowReadReceiptsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ShowReadReceiptsDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowReadReceiptsDialog() override;

    void setMessageId(const QByteArray &roomId);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    ShowReadReceiptsWidget *const mShowReadReceiptsWidget;
    RocketChatAccount *const mRocketChatAccount;
};
