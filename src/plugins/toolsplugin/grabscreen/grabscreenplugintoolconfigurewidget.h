/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "room/plugins/plugintoolconfigurewidget.h"

class GrabScreenPluginToolConfigureWidget : public PluginToolConfigureWidget
{
    Q_OBJECT
public:
    explicit GrabScreenPluginToolConfigureWidget(QWidget *parent = nullptr);
    ~GrabScreenPluginToolConfigureWidget() override;

    void save() override;
    void read() override;
};
