/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class ConfigureActivitiesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureActivitiesWidget(QWidget *parent = nullptr);
    ~ConfigureActivitiesWidget() override;

    [[nodiscard]] QStringList activities() const;
};
