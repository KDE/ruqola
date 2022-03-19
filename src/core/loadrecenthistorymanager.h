/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QDateTime>
#include <qglobal.h>

class LIBRUQOLACORE_TESTS_EXPORT LoadRecentHistoryManager
{
public:
    LoadRecentHistoryManager();
    ~LoadRecentHistoryManager();

    Q_REQUIRED_RESULT qint64 lastLoadingTimeStamp() const;

    Q_REQUIRED_RESULT qint64 generateNewStartTimeStamp(qint64 lastTimeStamp);

private:
    Q_DISABLE_COPY(LoadRecentHistoryManager)
    qint64 mLastLoadingTimeStamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
};
