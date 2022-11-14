/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class ConfigureAutoCorrectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureAutoCorrectionWidget(QWidget *parent = nullptr);
    ~ConfigureAutoCorrectionWidget() override;

    void save();
    void load();
};
