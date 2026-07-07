/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "abstractservercheck.h"

/** Sends a message containing a URL and verifies the server unfurls it into a link preview. */
class LIBRUQOLAWIDGETS_TESTS_EXPORT LinkPreviewCheck : public AbstractServerCheck
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
    QString mMarker;
};
