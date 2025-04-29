/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QListView>

class AiTextListView : public QListView
{
    Q_OBJECT
public:
    explicit AiTextListView(QWidget *parent = nullptr);
    ~AiTextListView() override;

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
};
