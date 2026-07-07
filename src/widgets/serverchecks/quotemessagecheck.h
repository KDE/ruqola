/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "abstractservercheck.h"

/** Sends a message, quotes it, and verifies the quoting message arrives with the original attached. */
class LIBRUQOLAWIDGETS_TESTS_EXPORT QuoteMessageCheck : public AbstractServerCheck
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
        WaitingForOriginal,
        WaitingForQuote,
    };
    Phase mPhase = Phase::WaitingForOriginal;
    QString mOriginalText;
    QString mQuoteMarker;
    QByteArray mOriginalId;
};
