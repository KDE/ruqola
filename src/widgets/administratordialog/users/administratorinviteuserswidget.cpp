/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorinviteuserswidget.h"

AdministratorInviteUsersWidget::AdministratorInviteUsersWidget(QWidget *parent)
    : QWidget{parent}
{
}

AdministratorInviteUsersWidget::~AdministratorInviteUsersWidget() = default;

QStringList AdministratorInviteUsersWidget::emails() const
{
    // TODO
    return {};
}
