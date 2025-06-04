/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class ConfigurePluginsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigurePluginsWidget(QWidget *parent = nullptr);
    ~ConfigurePluginsWidget() override;
};
