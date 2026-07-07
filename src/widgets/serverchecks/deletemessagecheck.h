/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "abstractservercheck.h"

/** Sends a message, deletes it, and verifies it disappears from the model. */
class LIBRUQOLAWIDGETS_TESTS_EXPORT DeleteMessageCheck : public AbstractServerCheck
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
        WaitingForDelete,
    };
    Phase mPhase = Phase::WaitingForSend;
    QString mMarker;
    QByteArray mMessageId;
};
