/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QObject>

class LIBRUQOLACORE_TESTS_EXPORT MessageDownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit MessageDownloadManager(QObject *parent = nullptr);
    ~MessageDownloadManager() override;
};
