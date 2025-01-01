/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "whatsnewtranslationbase.h"
#include <QCryptographicHash>
using namespace Qt::Literals::StringLiterals;
WhatsNewTranslationsBase::WhatsNewTranslationsBase() = default;

WhatsNewTranslationsBase::~WhatsNewTranslationsBase() = default;

QString WhatsNewTranslationsBase::newFeaturesMD5() const
{
    QByteArray str;
    for (const KLazyLocalizedString &l : lastNewFeatures()) {
        str += l.untranslatedText();
    }
    if (str.isEmpty()) {
        return {};
    }
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(str);
    return QLatin1StringView(md5.result().toBase64());
}
