/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QObject>
#include <texttranslator_version.h>
namespace TextTranslator
{
class TranslatorEnginePlugin;
}
class LIBRUQOLACORE_EXPORT TranslatorEngineManager : public QObject
{
    Q_OBJECT
public:
    ~TranslatorEngineManager() override;
    static TranslatorEngineManager *self();

    void translatorConfigChanged();

    [[nodiscard]] TextTranslator::TranslatorEnginePlugin *translatorEngineBase() const;

Q_SIGNALS:
    void updateTranslateEngine();
    void translateDone(const QString &translatedText);
    void translateFailed(const QString &errorMessage = QString());

private:
    explicit TranslatorEngineManager(QObject *parent = nullptr);
    LIBRUQOLACORE_NO_EXPORT void initializeTranslateEngine();
    LIBRUQOLACORE_NO_EXPORT void slotTranslateDone();
    TextTranslator::TranslatorEnginePlugin *mTranslatorEnginePlugin = nullptr;
};
