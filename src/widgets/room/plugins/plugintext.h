/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_export.h"
#include <QObject>
class PluginTextInterface;
class LIBRUQOLAWIDGETS_EXPORT PluginText : public QObject
{
    Q_OBJECT
public:
    explicit PluginText(QObject *parent = nullptr);
    ~PluginText() override;

    virtual PluginTextInterface *createInterface(QObject *parent) = 0;
};

