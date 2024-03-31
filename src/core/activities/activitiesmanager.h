/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ActivitiesManager : public QObject
{
    Q_OBJECT
public:
    explicit ActivitiesManager(QObject *parent = nullptr);
    ~ActivitiesManager() override;
};
