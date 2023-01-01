/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>

namespace Ui
{
class ConfigureAccountServerWidget;
}
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureAccountServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigureAccountServerWidget(QWidget *parent = nullptr);
    ~ConfigureAccountServerWidget() override;

    void load();
    void save();

private:
    void slotModifyServer();
    void slotAddServer();
    void slotDeleteServer();
    void slotItemSelectionChanged();

private:
    Ui::ConfigureAccountServerWidget *const ui;
};
