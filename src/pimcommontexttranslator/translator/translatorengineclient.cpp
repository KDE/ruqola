/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translatorengineclient.h"

using namespace PimCommonTextTranslator;

TranslatorEngineClient::TranslatorEngineClient(QObject *parent)
    : QObject{parent}
{
}

TranslatorEngineClient::~TranslatorEngineClient() = default;

bool TranslatorEngineClient::hasConfigurationDialog() const
{
    // False by default
    return false;
}

void TranslatorEngineClient::showConfigureDialog()
{
    // Nothing by default
}
