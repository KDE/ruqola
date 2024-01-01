/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_export.h"
#include <QObject>
class QMenu;
class LIBRUQOLAWIDGETS_EXPORT PluginTextInterface : public QObject
{
    Q_OBJECT
public:
    explicit PluginTextInterface(QObject *parent = nullptr);
    ~PluginTextInterface() override;

    virtual void addAction(QMenu *menu) = 0;

    virtual void setSelectedText(const QString &str);
};
