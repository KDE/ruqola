/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatorenginemanager.h"
#include <PimCommonTextTranslator/TranslatorEngineClient>
#include <PimCommonTextTranslator/TranslatorEngineLoader>
#include <PimCommonTextTranslator/TranslatorEnginePlugin>
#include <PimCommonTextTranslator/TranslatorUtil>
TranslatorEngineManager::TranslatorEngineManager(QObject *parent)
    : QObject{parent}
{
    initializeTranslateEngine();
}

TranslatorEngineManager::~TranslatorEngineManager() = default;

TranslatorEngineManager *TranslatorEngineManager::self()
{
    static TranslatorEngineManager s_self;
    return &s_self;
}

void TranslatorEngineManager::translatorConfigChanged()
{
    initializeTranslateEngine();
    Q_EMIT updateTranslateEngine();
}

void TranslatorEngineManager::initializeTranslateEngine()
{
    delete mTranslatorEnginePlugin;
    PimCommonTextTranslator::TranslatorEngineClient *translatorClient =
        PimCommonTextTranslator::TranslatorEngineLoader::self()->createTranslatorClient(PimCommonTextTranslator::TranslatorUtil::loadEngine());
    if (translatorClient) {
        mTranslatorEnginePlugin = translatorClient->createTranslator();
        connect(mTranslatorEnginePlugin, &PimCommonTextTranslator::TranslatorEnginePlugin::translateDone, this, &TranslatorEngineManager::slotTranslateDone);
        connect(mTranslatorEnginePlugin, &PimCommonTextTranslator::TranslatorEnginePlugin::translateFailed, this, &TranslatorEngineManager::translateFailed);
    } else {
        mTranslatorEnginePlugin = nullptr;
    }
}

PimCommonTextTranslator::TranslatorEnginePlugin *TranslatorEngineManager::translatorEngineBase() const
{
    return mTranslatorEnginePlugin;
}

void TranslatorEngineManager::slotTranslateDone()
{
    const QString result = mTranslatorEnginePlugin->resultTranslate();
    Q_EMIT translateDone(result);
}
