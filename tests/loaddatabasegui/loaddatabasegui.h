/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class LoadDataBaseGui : public QWidget
{
    Q_OBJECT
public:
    explicit LoadDataBaseGui(QWidget *parent = nullptr);
    ~LoadDataBaseGui() override = default;
};
