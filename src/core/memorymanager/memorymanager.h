/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

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
    void stopClearApplicationSettingsModelTimer();

Q_SIGNALS:
    void clearApplicationSettingsModelRequested();
    void cleanRoomHistoryRequested();

private:
    QTimer *const mClearApplicationSettingsModel;
    QTimer *const mClearRoomsHistory;
};
