/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QDialog>

class UnbanUsersDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UnbanUsersDialog(QWidget *parent = nullptr);
    ~UnbanUsersDialog() override;
};
