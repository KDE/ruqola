/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QDebug>
#include <QObject>
class RocketChatAccount;
class LIBRUQOLACORE_TESTS_EXPORT AwayManager : public QObject
{
    Q_OBJECT
public:
    explicit AwayManager(RocketChatAccount *const account, QObject *parent = nullptr);
    ~AwayManager() override;

    Q_REQUIRED_RESULT bool enabled() const;
    void setEnabled(bool newEnabled);

    Q_REQUIRED_RESULT int idleTiming() const;
    void setIdleTiming(int newIdleTiming);

    void updateSettings();

Q_SIGNALS:
    void awayChanged(bool away);

private:
    void slotResumeFromIdle();
    void slotIdleTimeoutReached(int timerId);
    Q_REQUIRED_RESULT int calculateRemainingTime() const;
    Q_REQUIRED_RESULT int timeValue() const;
    RocketChatAccount *const mRocketChatAccount;
    int mTimerId = -1;
    int mIdleTiming = -1;
    bool mEnabled = false;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AwayManager &t);
