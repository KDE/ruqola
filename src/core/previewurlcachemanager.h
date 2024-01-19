/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QObject>

class LIBRUQOLACORE_EXPORT PreviewUrlCacheManager : public QObject
{
    Q_OBJECT
public:
    explicit PreviewUrlCacheManager(QObject *parent = nullptr);
    ~PreviewUrlCacheManager() override;

    [[nodiscard]] int embedCacheExpirationDays() const;
    void setEmbedCacheExpirationDays(int newEmbedCacheExpirationDays);

private:
    int mEmbedCacheExpirationDays = -1;
};
