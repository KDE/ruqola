/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class DirectoryContainerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DirectoryContainerWidget(QWidget *parent = nullptr);
    ~DirectoryContainerWidget() override;
};
