/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QTreeView>
class LIBRUQOLAWIDGETS_TESTS_EXPORT PersonalAccessTokenTreeView : public QTreeView
{
public:
    explicit PersonalAccessTokenTreeView(QWidget *parent = nullptr);
    ~PersonalAccessTokenTreeView() override;
};
