/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "room/plugins/plugintextinterface.h"
class OllamaTextInterface : public PluginTextInterface
{
    Q_OBJECT
public:
    explicit OllamaTextInterface(QObject *parent = nullptr);
    ~OllamaTextInterface() override;

    void addAction(QMenu *menu) override;

    void setSelectedText(const QString &str) override;
};
