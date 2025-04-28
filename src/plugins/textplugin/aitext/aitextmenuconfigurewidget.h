/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QWidget>

class AiTextMenuConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AiTextMenuConfigureWidget(QWidget *parent = nullptr);
    ~AiTextMenuConfigureWidget() override;
};
