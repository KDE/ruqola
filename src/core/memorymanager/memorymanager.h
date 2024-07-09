/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MemoryManager : public QObject
{
    Q_OBJECT
public:
    explicit MemoryManager(QObject *parent = nullptr);
    ~MemoryManager() override;
};
