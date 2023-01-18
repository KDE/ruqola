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

#include <KSharedConfig>

#include <config-ruqola.h>

#if HAVE_TEXT_AUTOCORRECTION
#include "textautocorrection/autocorrection.h"
#include "textautocorrection/textautocorrectionsettings.h"
#endif

static Ruqola *s_self = nullptr;

Ruqola::Ruqola(QObject *parent)
    : QObject(parent)
#if HAVE_TEXT_AUTOCORRECTION
    , mAutoCorrection(new TextAutoCorrection::AutoCorrection())
#endif
{
    // Initialize paths
    (void)ManagerDataPaths::self();
    mAccountManager = new AccountManager(this);

#if HAVE_TEXT_AUTOCORRECTION
    TextAutoCorrection::TextAutoCorrectionSettings::self()->setSharedConfig(KSharedConfig::openConfig());
    TextAutoCorrection::TextAutoCorrectionSettings::self()->load();
    mAutoCorrection->readConfig();
#endif
}

Ruqola::~Ruqola()
{
#if HAVE_TEXT_AUTOCORRECTION
    delete mAutoCorrection;
#endif
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
