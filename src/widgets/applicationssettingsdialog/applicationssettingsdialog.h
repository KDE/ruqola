/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsDialog(QWidget *parent = nullptr);
    ~ApplicationsSettingsDialog() override;
};
