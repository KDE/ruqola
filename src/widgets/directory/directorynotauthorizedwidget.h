/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT DirectoryNotAuthorizedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DirectoryNotAuthorizedWidget(QWidget *parent = nullptr);
    ~DirectoryNotAuthorizedWidget() override;
};
