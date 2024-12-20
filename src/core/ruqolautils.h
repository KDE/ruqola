/*

 * SPDX-FileCopyrightText: 2017 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include "libruqolacore_export.h"
#include <QUrl>
class QString;

class LIBRUQOLACORE_EXPORT RuqolaUtils
{
public:
    ~RuqolaUtils();
    static RuqolaUtils *self();
    void openUrl(const QString &url);

    [[nodiscard]] QString extractRoomUserFromUrl(const QString &url);

    void openUrl(const QUrl &url);

private:
    RuqolaUtils();
};
