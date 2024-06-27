/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "whatsnewtranslationbase.h"
#include <KLazyLocalizedString>
#include <KLocalizedString>
#include <QCryptographicHash>

QString WhatsNewTranslationsBase::newFeaturesMD5()
{
    QByteArray str;
    for (const KLazyLocalizedString &l : lastNewFeatures()) {
        str += l.untranslatedText();
    }
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(str);
    return QLatin1StringView(md5.result().toBase64());
}

WhatsNewTranslationsBase::WhatsNewTranslationsBase() = default;

WhatsNewTranslationsBase::~WhatsNewTranslationsBase() = default;
