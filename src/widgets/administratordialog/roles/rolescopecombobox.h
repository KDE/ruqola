/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QComboBox>

class LIBRUQOLAWIDGETS_TESTS_EXPORT RoleScopeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit RoleScopeComboBox(QWidget *parent = nullptr);
    ~RoleScopeComboBox() override;
};
