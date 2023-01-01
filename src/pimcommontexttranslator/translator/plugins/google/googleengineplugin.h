/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <PimCommonTextTranslator/TranslatorEnginePlugin>
class QNetworkReply;
class GoogleEnginePlugin : public PimCommonTextTranslator::TranslatorEnginePlugin
{
public:
    explicit GoogleEnginePlugin(QObject *parent = nullptr);
    ~GoogleEnginePlugin() override;

    void translate() override;

private:
    void slotTranslateFinished(QNetworkReply *reply);
    QString mJsonData;
    QString mResult;
};
