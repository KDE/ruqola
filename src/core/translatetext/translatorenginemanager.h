/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QObject>
namespace PimCommonTextTranslator
{
class TranslatorEngineBase;
}
class LIBRUQOLACORE_EXPORT TranslatorEngineManager : public QObject
{
    Q_OBJECT
public:
    explicit TranslatorEngineManager(QObject *parent = nullptr);
    ~TranslatorEngineManager() override;
    static TranslatorEngineManager *self();

    void translatorConfigChanged();

    Q_REQUIRED_RESULT PimCommonTextTranslator::TranslatorEngineBase *translatorEngineBase() const;

Q_SIGNALS:
    void updateTranslateEngine();
    void translateDone(const QString &translatedText);
    void translateFailed(bool result, const QString &errorMessage = QString());

private:
    void initializeTranslateEngine();
    void slotTranslateDone();
    PimCommonTextTranslator::TranslatorEngineBase *mTranslatorEngineBase = nullptr;
};
