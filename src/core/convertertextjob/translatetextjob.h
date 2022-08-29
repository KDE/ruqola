/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "convertertextabstractjob.h"
#include "libruqola_private_export.h"
class LIBRUQOLACORE_TESTS_EXPORT TranslateTextJob : public ConverterTextAbstractJob
{
    Q_OBJECT
public:
    explicit TranslateTextJob(RocketChatAccount *account, QObject *parent = nullptr);
    ~TranslateTextJob() override;

    void start() override;
};
