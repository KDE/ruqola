/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "room/plugins/plugintoolconfigurewidget.h"
class QSpinBox;
class GrabScreenPluginToolConfigureWidget : public PluginToolConfigureWidget
{
    Q_OBJECT
public:
    explicit GrabScreenPluginToolConfigureWidget(QWidget *parent = nullptr);
    ~GrabScreenPluginToolConfigureWidget() override;

    void saveSettings() override;
    void loadSettings() override;

private:
    QSpinBox *const mDelay;
};
