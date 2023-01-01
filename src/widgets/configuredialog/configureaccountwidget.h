/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class ConfigureAccountServerWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureAccountWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureAccountWidget(QWidget *parent = nullptr);
    ~ConfigureAccountWidget() override;
    void save();
    void load();

private:
    ConfigureAccountServerWidget *const mConfigureAccountServerWidget;
};
