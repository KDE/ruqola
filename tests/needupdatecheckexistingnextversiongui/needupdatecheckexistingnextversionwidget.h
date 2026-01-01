/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class NeedUpdateCheckExistingNextVersionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NeedUpdateCheckExistingNextVersionWidget(QWidget *parent = nullptr);
    ~NeedUpdateCheckExistingNextVersionWidget() override = default;
};
