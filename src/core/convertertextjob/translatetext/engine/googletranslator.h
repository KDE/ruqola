/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "convertertextjob/translatetext/translatorenginebase.h"
#include "libruqolacore_export.h"

#include <QNetworkReply>
#include <QObject>
class QNetworkAccessManager;
class QComboBox;

class LIBRUQOLACORE_EXPORT GoogleTranslator : public TranslatorEngineBase
{
    Q_OBJECT
public:
    explicit GoogleTranslator(QObject *parent = nullptr);
    ~GoogleTranslator() override;

    void translate() override;

    Q_REQUIRED_RESULT QString engineName() const override;
    Q_REQUIRED_RESULT QVector<QPair<QString, QString>> supportedLanguage() const override;

    Q_REQUIRED_RESULT static QVector<QPair<QString, QString>> languages();

private:
    void slotTranslateFinished(QNetworkReply *);
    static inline QVector<QPair<QString, QString>> mLanguages;
};
