/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountinfosource.h"
#include "accountmanager.h"
#include "ruqola.h"
#include <KLocalizedString>
#include <QVariant>

AccountInfoSource::AccountInfoSource()
    : KUserFeedback::AbstractDataSource(QStringLiteral("accounts"), KUserFeedback::Provider::DetailedSystemInformation)
{
}

AccountInfoSource::~AccountInfoSource() = default;

QString AccountInfoSource::name() const
{
    return i18n("Account information");
}

QString AccountInfoSource::description() const
{
    return i18n("Return the number of account.");
}

QVariant AccountInfoSource::data()
{
    QVariantMap m;
    m.insert(QStringLiteral("value"), Ruqola::self()->accountManager()->accountNumber());
    return m;
}
