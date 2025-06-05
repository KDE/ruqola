/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_export.h"
#include <QWidget>

class LIBRUQOLAWIDGETS_EXPORT PluginToolConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PluginToolConfigureWidget(QWidget *parent = nullptr);
    ~PluginToolConfigureWidget() override;
    virtual void saveSettings() = 0;
    virtual void loadSettings() = 0;
};
