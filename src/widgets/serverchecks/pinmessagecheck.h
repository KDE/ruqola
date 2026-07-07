/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "abstractservercheck.h"

/** Sends a message, pins it, and verifies it shows up in the room's pinned messages. */
class LIBRUQOLAWIDGETS_TESTS_EXPORT PinMessageCheck : public AbstractServerCheck
{
    Q_OBJECT
public:
    using AbstractServerCheck::AbstractServerCheck;

    [[nodiscard]] QString name() const override;

protected:
    void doStart() override;
    void checkState() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void startPin();
    LIBRUQOLAWIDGETS_NO_EXPORT void verifyPinned();
    enum class Phase : uint8_t {
        WaitingForSend,
        Working,
    };
    Phase mPhase = Phase::WaitingForSend;
    QString mMarker;
    QByteArray mMessageId;
};
