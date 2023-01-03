/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "pimcommontexttranslator_export.h"
#include <QObject>
class QNetworkAccessManager;
namespace PimCommonTextTranslator
{
class PIMCOMMONTEXTTRANSLATOR_EXPORT TranslatorEngineAccessManager : public QObject
{
    Q_OBJECT
public:
    explicit TranslatorEngineAccessManager(QObject *parent = nullptr);
    ~TranslatorEngineAccessManager();

    static TranslatorEngineAccessManager *self();

    QNetworkAccessManager *networkManager();

private:
    QNetworkAccessManager *const mNetworkAccessManager;
};
}
