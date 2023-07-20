/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include <QObject>

class LIBRUQOLACORE_TESTS_EXPORT ManageLoadHistory : public QObject
{
    Q_OBJECT
public:
    explicit ManageLoadHistory(QObject *parent = nullptr);
    ~ManageLoadHistory() override;
};
