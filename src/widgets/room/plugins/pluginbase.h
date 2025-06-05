/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_export.h"
#include <QObject>

class LIBRUQOLAWIDGETS_EXPORT PluginBase : public QObject
{
    Q_OBJECT
public:
    explicit PluginBase(QObject *parent = nullptr);
    ~PluginBase() override;
    [[nodiscard]] bool enabled() const;
    void setEnabled(bool newEnabled);

    [[nodiscard]] virtual bool hasConfigureDialog() const;
    virtual void showConfigureDialog(QWidget *parent = nullptr) const;

protected:
    bool mEnabled = true;
};
