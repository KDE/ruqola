/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QWidget>

class ApplicationsSettingsDescriptionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsDescriptionWidget(QWidget *parent = nullptr);
    ~ApplicationsSettingsDescriptionWidget() override;
};
