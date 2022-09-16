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

    Q_REQUIRED_RESULT const QString &from() const;
    void setFrom(const QString &newFrom);

    Q_REQUIRED_RESULT const QString &to() const;
    void setTo(const QString &newTo);

    Q_REQUIRED_RESULT const QString &inputText() const;
    void setInputText(const QString &newInputText);

Q_SIGNALS:
    void translateDone(const QString &translatedText);
    void translateFailed(bool result, const QString &errorMessage = QString());

private:
    void slotTranslateDone();
    QString mFrom;
    QString mTo;
    QString mInputText;
    TranslatorEngineBase *mTranslatorEngineBase = nullptr;
};
