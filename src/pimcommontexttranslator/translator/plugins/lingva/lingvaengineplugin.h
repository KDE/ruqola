/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <PimCommonTextTranslator/TranslatorEnginePlugin>

class LingvaEnginePlugin : public PimCommonTextTranslator::TranslatorEnginePlugin
{
    Q_OBJECT
public:
    explicit LingvaEnginePlugin(QObject *parent = nullptr);
    ~LingvaEnginePlugin() override;

    void translate() override;

    void slotConfigureChanged();

private:
    void loadSettings();
    void translateText();
    void parseTranslation(QNetworkReply *reply);
    QString mServerUrl;
    QString mResult;
};
