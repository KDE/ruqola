/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QTreeView>

#include "libruqolawidgets_private_export.h"
class LIBRUQOLAWIDGETS_TESTS_EXPORT PermissionsTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit PermissionsTreeView(QWidget *parent = nullptr);
    ~PermissionsTreeView() override;
};
