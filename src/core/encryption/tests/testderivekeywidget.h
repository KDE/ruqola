/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QWidget>

class TestDeriveKeyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TestDeriveKeyWidget(QWidget *parent = nullptr);
    ~TestDeriveKeyWidget() override = default;
};
