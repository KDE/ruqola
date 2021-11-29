/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "convertertextabstractjob.h"
#include "libruqola_private_export.h"
class LIBRUQOLACORE_TESTS_EXPORT TranslateTextJob : public ConverterTextAbstractJob
{
    Q_OBJECT
public:
    explicit TranslateTextJob(QObject *parent = nullptr);
    ~TranslateTextJob() override;

    void start() override;
};

