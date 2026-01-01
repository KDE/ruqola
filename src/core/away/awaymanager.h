/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QObject>
class RocketChatAccount;
class LIBRUQOLACORE_TESTS_EXPORT AwayManager : public QObject
{
    Q_OBJECT
public:
    explicit AwayManager(RocketChatAccount *const account, QObject *parent = nullptr);
    ~AwayManager() override;

    [[nodiscard]] bool enabled() const;
    void setEnabled(bool newEnabled);

    [[nodiscard]] int idleTiming() const;
    void setIdleTiming(int newIdleTiming);

    void updateSettings();

Q_SIGNALS:
    void awayChanged(bool away);

private:
    LIBRUQOLACORE_NO_EXPORT void slotResumeFromIdle();
    LIBRUQOLACORE_NO_EXPORT void slotIdleTimeoutReached(int timerId);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT int calculateRemainingTime() const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT int timeValue() const;
    RocketChatAccount *const mRocketChatAccount;
    int mTimerId = -1;
    int mIdleTiming = -1;
    bool mEnabled = false;
};
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AwayManager &t);
