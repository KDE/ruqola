/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_export.h"
#include "pluginbase.h"
class PluginTextInterface;
class LIBRUQOLAWIDGETS_EXPORT PluginText : public PluginBase
{
    Q_OBJECT
public:
    explicit PluginText(QObject *parent = nullptr);
    ~PluginText() override;

    virtual PluginTextInterface *createInterface(QWidget *parentWidget, QObject *parent) = 0;

    [[nodiscard]] virtual int order() const = 0;
Q_SIGNALS:
    void errorMessage(const QString &message);
    void successMessage(const QString &message);
};
