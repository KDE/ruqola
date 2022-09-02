/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "convertertextjob/translatetext/translatorenginebase.h"
class DeepLTranslator : public TranslatorEngineBase
{
    Q_OBJECT
public:
    explicit DeepLTranslator(QObject *parent = nullptr);
    ~DeepLTranslator() override;

    Q_REQUIRED_RESULT QString engineName() const override;
    void translate() override;
    Q_REQUIRED_RESULT QVector<QPair<QString, QString>> supportedLanguage() const override;
    void loadSettings() override;

private:
    Q_REQUIRED_RESULT QString apiUrl() const;
    void translateText();
    void parseTranslation(QNetworkReply *reply);
    bool mUseFreeLicense = false;
};
