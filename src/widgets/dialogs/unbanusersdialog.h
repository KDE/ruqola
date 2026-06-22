/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_export.h"
#include <QDialog>

class LIBRUQOLAWIDGETS_EXPORT UnbanUsersDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UnbanUsersDialog(QWidget *parent = nullptr);
    ~UnbanUsersDialog() override;
};
