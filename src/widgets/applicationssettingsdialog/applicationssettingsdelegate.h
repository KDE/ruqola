/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QItemDelegate>

class ApplicationsSettingsDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsDelegate(QObject *parent = nullptr);
    ~ApplicationsSettingsDelegate() override;
};
