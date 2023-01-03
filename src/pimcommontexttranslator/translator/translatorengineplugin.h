/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimcommontexttranslator_export.h"
#include <PimCommonTextTranslator/TranslatorUtil>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <memory>

namespace PimCommonTextTranslator
{
class TranslatorEnginePluginPrivate;
class PIMCOMMONTEXTTRANSLATOR_EXPORT TranslatorEnginePlugin : public QObject
{
    Q_OBJECT
public:
    explicit TranslatorEnginePlugin(QObject *parent = nullptr);
    ~TranslatorEnginePlugin() override;

    virtual void translate() = 0;

    Q_REQUIRED_RESULT QString resultTranslate() const;
    void setInputText(const QString &text);
    void setFrom(const QString &language);
    void setTo(const QString &language);
    void setResult(const QString &result);
    void setJsonDebug(const QString &debug);

    Q_REQUIRED_RESULT QString inputText() const;
    Q_REQUIRED_RESULT QString from() const;
    Q_REQUIRED_RESULT QString to() const;
    Q_REQUIRED_RESULT QString result() const;
    Q_REQUIRED_RESULT QString jsonDebug() const;

    void clear();

Q_SIGNALS:
    void translateDone();
    void translateFailed(bool result, const QString &errorMessage = QString());

protected:
    void appendResult(const QString &result);
    void slotError(QNetworkReply::NetworkError error);
    Q_REQUIRED_RESULT bool verifyFromAndToLanguage();
    Q_REQUIRED_RESULT bool hasDebug() const;

    Q_REQUIRED_RESULT virtual QString languageCode(const QString &langStr);

private:
    std::unique_ptr<TranslatorEnginePluginPrivate> const d;
};
}
