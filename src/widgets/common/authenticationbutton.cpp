/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationbutton.h"

AuthenticationButton::AuthenticationButton(QWidget *parent)
    : QPushButton(parent)
{
}

AuthenticationButton::~AuthenticationButton() = default;

#include "moc_authenticationbutton.cpp"
