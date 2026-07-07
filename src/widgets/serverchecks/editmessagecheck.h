/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "abstractservercheck.h"

/** Sends a message, edits it, and verifies the edited text arrives. */
class LIBRUQOLAWIDGETS_TESTS_EXPORT EditMessageCheck : public AbstractServerCheck
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
        WaitingForInitial,
        WaitingForEdit,
    };
    Phase mPhase = Phase::WaitingForInitial;
    QString mInitialText;
    QString mEditedText;
    QByteArray mMessageId;
};
