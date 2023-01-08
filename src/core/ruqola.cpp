/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "ruqola.h"
#include "accountmanager.h"
#include "managerdatapaths.h"
#include "rocketchataccount.h"
#include "textautocorrection/autocorrection.h"
#include "textautocorrection/textautocorrectionsettings.h"

#include <KSharedConfig>

static Ruqola *s_self = nullptr;

Ruqola::Ruqola(QObject *parent)
    : QObject(parent)
    , mAutoCorrection(new TextAutoCorrection::AutoCorrection())
{
    // Initialize paths
    (void)ManagerDataPaths::self();
    mAccountManager = new AccountManager(this);
    TextAutoCorrection::TextAutoCorrectionSettings::self()->setSharedConfig(KSharedConfig::openConfig());
    TextAutoCorrection::TextAutoCorrectionSettings::self()->load();
    mAutoCorrection->readConfig();
}

Ruqola::~Ruqola()
{
    delete mAutoCorrection;
}

Ruqola *Ruqola::self()
{
    if (!s_self) {
        s_self = new Ruqola;
    }
    return s_self;
}

void Ruqola::destroy()
{
    delete s_self;
    s_self = nullptr;
}

void Ruqola::openMessageUrl(const QString &url)
{
    mAccountManager->openMessageUrl(url);
}

TextAutoCorrection::AutoCorrection *Ruqola::autoCorrection() const
{
    return mAutoCorrection;
}

void Ruqola::setCurrentAccount(const QString &accountName)
{
    mAccountManager->setCurrentAccount(accountName);
}

AccountManager *Ruqola::accountManager() const
{
    return mAccountManager;
}

RocketChatAccount *Ruqola::rocketChatAccount() const
{
    return mAccountManager->account();
}
