/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class CreateVideoMessageWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateVideoMessageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateVideoMessageDialog(QWidget *parent = nullptr);
    ~CreateVideoMessageDialog() override;

private:
    CreateVideoMessageWidget *const mCreateVideoMessageWidget;
};

