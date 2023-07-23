/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class OffLineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OffLineWidget(QWidget *parent = nullptr);
    ~OffLineWidget() override;
};
