/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "abstractservercheck.h"

/** Sends a message, adds an emoji reaction, and verifies the reaction arrives on the message. */
class LIBRUQOLAWIDGETS_TESTS_EXPORT ReactionCheck : public AbstractServerCheck
{
    Q_OBJECT
public:
    using AbstractServerCheck::AbstractServerCheck;

    [[nodiscard]] QString name() const override;

protected:
    void doStart() override;
    void checkState() override;

private:
    enum class Phase : uint8_t {
        WaitingForSend,
        WaitingForReaction,
    };
    Phase mPhase = Phase::WaitingForSend;
    QString mMarker;
    QByteArray mMessageId;
};
