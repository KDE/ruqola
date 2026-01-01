/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class AuthenticationLoginWidget;
class RestAuthenticationGui : public QWidget
{
    Q_OBJECT
public:
    explicit RestAuthenticationGui(QWidget *parent = nullptr);
    ~RestAuthenticationGui() override;

private:
    AuthenticationLoginWidget *const mAuthenticationLoginWidget;
};
