/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "abstractservercheck.h"

/** Sends a message with a unique token, then searches for it to verify server-side indexing. */
class LIBRUQOLAWIDGETS_TESTS_EXPORT SearchMessageCheck : public AbstractServerCheck
{
    Q_OBJECT
public:
    using AbstractServerCheck::AbstractServerCheck;

    [[nodiscard]] QString name() const override;

protected:
    void doStart() override;
    void checkState() override;
    [[nodiscard]] int watchdogTimeoutMs() const override;

private:
    enum class Phase : uint8_t {
        WaitingForSend,
        Searching,
    };
    LIBRUQOLAWIDGETS_NO_EXPORT void startSearch();
    Phase mPhase = Phase::WaitingForSend;
    QString mToken;
    QByteArray mMessageId;
};
