/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QComboBox>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ServersComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ServersComboBox(QWidget *parent = nullptr);
    ~ServersComboBox() override;
};
