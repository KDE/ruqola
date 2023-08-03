/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "awaymanager.h"
#include "rocketchataccount.h"
#include "ruqola_away_debug.h"
#include <KIdleTime>
// #define DEBUG_IDLETIME 1
AwayManager::AwayManager(RocketChatAccount *const account, QObject *parent)
    : QObject{parent}
    , mRocketChatAccount(account)
{
    connect(KIdleTime::instance(), &KIdleTime::resumingFromIdle, this, &AwayManager::slotResumeFromIdle);
    connect(KIdleTime::instance(), &KIdleTime::timeoutReached, this, &AwayManager::slotIdleTimeoutReached);

    KIdleTime::instance()->catchNextResumeEvent();
}

AwayManager::~AwayManager() = default;

void AwayManager::slotResumeFromIdle()
{
    if (!mEnabled) {
        return;
    }
    Q_EMIT awayChanged(false); // Not away now
    qCDebug(RUQOLA_AWAY_LOG) << " void AwayManager::slotResumeFromIdle() : name : " << mRocketChatAccount->accountName();
    const QHash<int, int> idleTimeouts = KIdleTime::instance()->idleTimeouts();

    int remainingTime = calculateRemainingTime();
    // Check if the user should be away right now.
    if (remainingTime <= 0) {
        Q_EMIT awayChanged(true);
        // As at least one identity is away we have to catch the next
        // resume event.
        KIdleTime::instance()->catchNextResumeEvent();

        // Since the user is away right now the next auto-away should occur
        // in X minutes (where X is the timeout which the user has
        // configured for the identity).
        remainingTime = mIdleTiming;
    }
    if (idleTimeouts[mTimerId] != remainingTime) {
        // Remove the idle timeout.
        if (mTimerId != -1) {
            KIdleTime::instance()->removeIdleTimeout(mTimerId);
        }

        // Then also reset the timer ID (as the timer does not exist anymore).
        mTimerId = -1;
    }
    // Check if we already have a timer.
    if (mTimerId == -1) {
        mTimerId = KIdleTime::instance()->addIdleTimeout(timeValue());
    }
}

int AwayManager::calculateRemainingTime() const
{
    // The remaining time until the user will be marked as "auto-away".
    const int remainingTime = mIdleTiming - KIdleTime::instance()->idleTime();

    return remainingTime;
}

void AwayManager::slotIdleTimeoutReached(int timerId)
{
    if (!mEnabled) {
        return;
    }
    if (mTimerId == timerId) {
        qDebug() << " slotIdleTimeoutReached " << KIdleTime::instance()->idleTimeouts();
        qCDebug(RUQOLA_AWAY_LOG) << " void AwayManager::slotIdleTimeoutReached() mTimerId :" << mTimerId << "name : " << mRocketChatAccount->accountName();
        Q_EMIT awayChanged(true); // Away now
        // Account is away => we need to catch next resume event.
        KIdleTime::instance()->catchNextResumeEvent();
    }
}

int AwayManager::idleTiming() const
{
    return mIdleTiming;
}

void AwayManager::setIdleTiming(int newIdleTiming)
{
    mIdleTiming = newIdleTiming;
}

void AwayManager::updateSettings()
{
    const auto &ownerUserPref = mRocketChatAccount->ownUserPreferences();
    setIdleTiming(ownerUserPref.idleTimeLimit());
    setEnabled(ownerUserPref.enableAutoAway());
}

bool AwayManager::enabled() const
{
    return mEnabled;
}

void AwayManager::setEnabled(bool newEnabled)
{
    qCDebug(RUQOLA_AWAY_LOG) << " void AwayManager::setEnabled()" << newEnabled << "name : " << mRocketChatAccount->accountName();
    if (mEnabled != newEnabled) {
        mEnabled = newEnabled;
        if (!mEnabled && (mTimerId != -1)) {
            KIdleTime::instance()->removeIdleTimeout(mTimerId);
            qCDebug(RUQOLA_AWAY_LOG) << " Remove Idle Timeout " << newEnabled;
            mTimerId = -1;
        } else if (mEnabled && (mTimerId == -1)) {
            mTimerId = KIdleTime::instance()->addIdleTimeout(timeValue());
            qCDebug(RUQOLA_AWAY_LOG) << " Catch Next Resume Event " << newEnabled << " mIdleTiming (s)" << mIdleTiming << " mTimerId " << mTimerId;
            KIdleTime::instance()->catchNextResumeEvent();
        }
    }
}

int AwayManager::timeValue() const
{
    constexpr int minute = 60 * 1000;
#ifdef DEBUG_IDLETIME
    const int timerValue = minute; // 1 minutes
#else
    const int timerValue = mIdleTiming * minute;
#endif
    return timerValue;
}

QDebug operator<<(QDebug d, const AwayManager &t)
{
    d << "mEnabled " << t.enabled();
    d << "mIdleTiming " << t.idleTiming();
    return d;
}

#include "moc_awaymanager.cpp"
