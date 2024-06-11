/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QComboBox>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsStatusComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsStatusComboBox(QWidget *parent = nullptr);
    ~ApplicationsSettingsStatusComboBox() override;
};
