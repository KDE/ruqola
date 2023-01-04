/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "libretranslateengineconfigurewidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QCompleter>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

LibreTranslateEngineConfigureWidget::LibreTranslateEngineConfigureWidget(QWidget *parent)
    : QWidget{parent}
    , mServerUrl(new QLineEdit(this))
    , mApiKey(new QLineEdit(this))
    , mRequiredApiKey(new QCheckBox(i18n("Server required Api Key"), this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mServerUrl->setObjectName(QStringLiteral("mServerUrl"));
    mServerUrl->setClearButtonEnabled(true);
    mainLayout->addRow(i18n("Server Url:"), mServerUrl);

    mRequiredApiKey->setObjectName(QStringLiteral("mRequiredApiKey"));
    mainLayout->addWidget(mRequiredApiKey);

    mApiKey->setObjectName(QStringLiteral("mApiKey"));
    mApiKey->setClearButtonEnabled(true);
    mainLayout->addRow(i18n("Api Key:"), mApiKey);
    connect(mRequiredApiKey, &QCheckBox::clicked, this, &LibreTranslateEngineConfigureWidget::updateApiKeyState);

    const QStringList listServer{
        QStringLiteral("https://libretranslate.com"),
        QStringLiteral("https://libretranslate.de"),
        QStringLiteral("https://translate.argosopentech.com"),
        QStringLiteral("https://translate.api.skitzen.com"),
        QStringLiteral("https://translate.fortytwo-it.com"),
        QStringLiteral("https://translate.terraprint.co"),
        QStringLiteral("https://lt.vern.cc"),
    };

    auto completer = new QCompleter(listServer, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    mServerUrl->setCompleter(completer);
}

LibreTranslateEngineConfigureWidget::~LibreTranslateEngineConfigureWidget() = default;

void LibreTranslateEngineConfigureWidget::updateApiKeyState(bool state)
{
    mApiKey->setEnabled(state);
}

QString LibreTranslateEngineConfigureWidget::serverUrl() const
{
    return mServerUrl->text();
}

void LibreTranslateEngineConfigureWidget::setServerUrl(const QString &serverUrl)
{
    mServerUrl->setText(serverUrl);
}

QString LibreTranslateEngineConfigureWidget::apiKey() const
{
    return mApiKey->text();
}

void LibreTranslateEngineConfigureWidget::setApiKey(const QString &key)
{
    mApiKey->setText(key);
}

bool LibreTranslateEngineConfigureWidget::serverRequiredApiKey() const
{
    return mRequiredApiKey->isChecked();
}

void LibreTranslateEngineConfigureWidget::setServerRequiredApiKey(bool state)
{
    mRequiredApiKey->setChecked(state);
    updateApiKeyState(state);
}
