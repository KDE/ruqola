/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QPlainTextEdit>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ViewLogPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit ViewLogPlainTextEdit(QWidget *parent = nullptr);
    ~ViewLogPlainTextEdit() override;
};
