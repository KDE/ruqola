/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QObject>

class PluginToolInterface : public QObject
{
    Q_OBJECT
public:
    explicit PluginToolInterface(QObject *parent = nullptr);
    ~PluginToolInterface() override;
};
