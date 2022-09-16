/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "convertertextabstractjob.h"
#include "libruqolacore_export.h"
class TranslatorEngineBase;
class LIBRUQOLACORE_EXPORT TranslateTextJob : public ConverterTextAbstractJob
{
    Q_OBJECT
public:
    explicit TranslateTextJob(RocketChatAccount *account, QObject *parent = nullptr);
    ~TranslateTextJob() override;

    void start() override;

private:
    TranslatorEngineBase *mTranslatorEngineBase = nullptr;
};
