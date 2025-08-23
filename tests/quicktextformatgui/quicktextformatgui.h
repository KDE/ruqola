/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QtWidgets/QWidget>

class QuickTextFormatGui : public QWidget
{
    Q_OBJECT
public:
    explicit QuickTextFormatGui(QWidget *parent = nullptr);
    ~QuickTextFormatGui() override;
};
