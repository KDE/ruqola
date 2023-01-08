/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <TextTranslator/TranslatorEnginePlugin>

class YandexEnginePlugin : public TextTranslator::TranslatorEnginePlugin
{
    Q_OBJECT
public:
    explicit YandexEnginePlugin(QObject *parent = nullptr);
    ~YandexEnginePlugin() override;

    void translate() override;

    Q_REQUIRED_RESULT QString languageCode(const QString &langStr) override;

private:
    void parseCredentials(QNetworkReply *reply);
    void translateText();
    void parseTranslation(QNetworkReply *reply);
    static QString sYandexKey;
    QString mResult;
};
