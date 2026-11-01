/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QMenu>

class LIBRUQOLAWIDGETS_TESTS_EXPORT CategoriesMenu : public QMenu
{
    Q_OBJECT
public:
    explicit CategoriesMenu(QWidget *parent = nullptr);
    ~CategoriesMenu() override;
};
