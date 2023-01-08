/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <TextTranslator/TranslatorEnginePlugin>

class LibreTranslateEnginePlugin : public TextTranslator::TranslatorEnginePlugin
{
    Q_OBJECT
public:
    explicit LibreTranslateEnginePlugin(QObject *parent = nullptr);
    ~LibreTranslateEnginePlugin() override;

    void translate() override;

    void slotConfigureChanged();

private:
    void loadSettings();
    void parseTranslation(QNetworkReply *reply);
    void translateText();
    QString mServerUrl;
    QString mResult;
    // TODO load it ? kwallet ?
    QString mApiKey;
    bool mRequiredApiKey = false;
};
