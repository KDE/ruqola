/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QWidget>

class PluginToolConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PluginToolConfigureWidget(QWidget *parent = nullptr);
    ~PluginToolConfigureWidget() override;
    virtual void save() = 0;
    virtual void read() = 0;
};
