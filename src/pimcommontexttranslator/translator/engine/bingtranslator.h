/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "translator/translatorenginebase.h"
namespace PimCommonTextTranslator
{
class BingTranslator : public TranslatorEngineBase
{
    Q_OBJECT
public:
    explicit BingTranslator(QObject *parent = nullptr);
    ~BingTranslator() override;

    void translate() override;
    Q_REQUIRED_RESULT QString engineName() const override;
    Q_REQUIRED_RESULT QVector<QPair<QString, QString>> supportedLanguage() override;
    Q_REQUIRED_RESULT static QVector<QPair<QString, QString>> languages();

private:
    void parseCredentials(QNetworkReply *reply);
    void parseTranslation(QNetworkReply *reply);
    void translateText();
    static inline QVector<QPair<QString, QString>> mLanguages;
    static QByteArray sBingKey;
    static QByteArray sBingToken;
    static QString sBingIg;
    static QString sBingIid;
};
}
