/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QObject>
class QTimer;
class LIBRUQOLACORE_EXPORT MemoryManager : public QObject
{
    Q_OBJECT
public:
    explicit MemoryManager(QObject *parent = nullptr);
    ~MemoryManager() override;

    void startClearApplicationSettingsModelTimer();

Q_SIGNALS:
    void ClearApplicationSettingsModelRequested();

private:
    QTimer *const mClearApplicationSettingsModel;
};
