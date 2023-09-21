/*

 * SPDX-FileCopyrightText: 2017 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include "libruqolacore_export.h"
#include <QObject>

class QString;

class LIBRUQOLACORE_EXPORT RuqolaUtils : public QObject
{
    Q_OBJECT
public:
    ~RuqolaUtils() override;
    static RuqolaUtils *self();
    void openUrl(const QString &url);

    [[nodiscard]] QString extractRoomUserFromUrl(const QString &url);

private:
    explicit RuqolaUtils(QObject *parent = nullptr);
    Q_DISABLE_COPY(RuqolaUtils)
};
