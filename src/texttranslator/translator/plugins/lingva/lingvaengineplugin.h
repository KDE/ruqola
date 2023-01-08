/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <TextTranslator/TranslatorEnginePlugin>

class LingvaEnginePlugin : public TextTranslator::TranslatorEnginePlugin
{
    Q_OBJECT
public:
    explicit LingvaEnginePlugin(QObject *parent = nullptr);
    ~LingvaEnginePlugin() override;

    void translate() override;

    void slotConfigureChanged();

    Q_REQUIRED_RESULT QString languageCode(const QString &langStr) override;

private:
    void loadSettings();
    void translateText();
    void parseTranslation(QNetworkReply *reply);
    QString mServerUrl;
    QString mResult;
};
