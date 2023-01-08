/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <TextTranslator/TranslatorEnginePlugin>

class DeeplEnginePlugin : public TextTranslator::TranslatorEnginePlugin
{
    Q_OBJECT
public:
    explicit DeeplEnginePlugin(QObject *parent = nullptr);
    ~DeeplEnginePlugin() override;

    void translate() override;

    void slotConfigureChanged();

protected:
    Q_REQUIRED_RESULT QString languageCode(const QString &langStr) override;

private:
    void loadSettings();
    void translateText();
    void parseTranslation(QNetworkReply *reply);
    QString mServerUrl;
    QString mResult;
    // TODO load it ? kwallet ?
    QString mApiKey;
    bool mUseFreeLicense = false;
};
