/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <PimCommonTextTranslator/TranslatorEnginePlugin>

class DeeplEnginePlugin : public PimCommonTextTranslator::TranslatorEnginePlugin
{
    Q_OBJECT
public:
    explicit DeeplEnginePlugin(QObject *parent = nullptr);
    ~DeeplEnginePlugin() override;

    void translate() override;

    void slotConfigureChanged();

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
