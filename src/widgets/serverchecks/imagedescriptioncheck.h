/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "abstractservercheck.h"

/**
 * Uploads a small image with a description and verifies the description arrives
 * on the message attachment. This is a known-buggy area on some server versions
 * (image arrives, description dropped).
 */
class LIBRUQOLAWIDGETS_TESTS_EXPORT ImageDescriptionCheck : public AbstractServerCheck
{
    Q_OBJECT
public:
    using AbstractServerCheck::AbstractServerCheck;

    [[nodiscard]] QString name() const override;

protected:
    void doStart() override;
    void checkState() override;

private:
    QString mDescription;
    QString mFileName;
};
