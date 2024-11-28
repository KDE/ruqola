/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QWidget>

class ApplicationsSettingsListViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsListViewWidget(QWidget *parent = nullptr);
    ~ApplicationsSettingsListViewWidget() override;
};
