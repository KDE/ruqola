/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class PasswordValidateWidget;
class PasswordValidateGui : public QWidget
{
    Q_OBJECT
public:
    explicit PasswordValidateGui(QWidget *parent = nullptr);
    ~PasswordValidateGui() override;

private:
    PasswordValidateWidget *const mPasswordValidateWidget;
};
