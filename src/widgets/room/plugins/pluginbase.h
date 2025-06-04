/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class PluginBase : public QObject
{
    Q_OBJECT
public:
    explicit PluginBase(QObject *parent = nullptr);
    ~PluginBase() override;
    [[nodiscard]] bool enabled() const;
    void setEnabled(bool newEnabled);

protected:
    bool mEnabled = true;
};
