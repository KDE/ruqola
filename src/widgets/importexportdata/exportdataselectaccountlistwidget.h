/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QListWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ExportDataSelectAccountListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ExportDataSelectAccountListWidget(QWidget *parent = nullptr);
    ~ExportDataSelectAccountListWidget() override;
};
