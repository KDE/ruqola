/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ExtractServerInfoJob : public QObject
{
    Q_OBJECT
public:
    explicit ExtractServerInfoJob(QObject *parent = nullptr);
    ~ExtractServerInfoJob() override;

    void start();
};
