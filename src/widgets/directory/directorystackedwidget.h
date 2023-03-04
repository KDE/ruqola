/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QStackedWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT DirectoryStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit DirectoryStackedWidget(QWidget *parent = nullptr);
    ~DirectoryStackedWidget() override;
};
