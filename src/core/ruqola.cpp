/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "ruqola.h"
#include "accountmanager.h"
#include "customemojiiconmanager.h"
#include "managerdatapaths.h"
#include "rocketchataccount.h"

#include <KSharedConfig>

#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
#include "textautocorrectioncore/textautocorrectionsettings.h"
#include <TextAutoCorrectionCore/AutoCorrection>
#endif

static Ruqola *s_self = nullptr;

Ruqola::Ruqola(QObject *parent)
    : QObject(parent)
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    , mAutoCorrection(new TextAutoCorrectionCore::AutoCorrection())
#endif
    , mCustomEmojiIconManager(new CustomEmojiIconManager(this))
{
    mDebug = !qEnvironmentVariableIsEmpty("RUQOLA_DEBUGGING");
    // Initialize paths
    (void)ManagerDataPaths::self();
    mAccountManager = new AccountManager(this);

#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    TextAutoCorrectionCore::TextAutoCorrectionSettings::self()->setSharedConfig(KSharedConfig::openConfig());
    TextAutoCorrectionCore::TextAutoCorrectionSettings::self()->load();
    mAutoCorrection->readConfig();
#endif
}

Ruqola::~Ruqola()
{
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
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

#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
TextAutoCorrectionCore::AutoCorrection *Ruqola::autoCorrection() const
{
    return mAutoCorrection;
}
#endif

CustomEmojiIconManager *Ruqola::customEmojiIconManager() const
{
    return mCustomEmojiIconManager;
}

bool Ruqola::debug() const
{
    return mDebug;
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

#include "moc_ruqola.cpp"
