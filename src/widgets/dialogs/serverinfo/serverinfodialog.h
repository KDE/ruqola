/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ServerInfoWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ServerInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ServerInfoDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ServerInfoDialog() override;

private:
    ServerInfoWidget *const mServerInfoWidget;
};
