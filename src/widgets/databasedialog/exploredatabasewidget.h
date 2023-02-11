/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseWidget(QWidget *parent = nullptr);
    ~ExploreDatabaseWidget() override;
};
