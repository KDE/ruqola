/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "ruqola.h"

#include "customemojiiconmanager.h"
#include "managerdatapaths.h"
#include "rocketchataccount.h"

#include <KSharedConfig>

#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
#include "textautocorrectioncore/textautocorrectionsettings.h"
#include <TextAutoCorrectionCore/AutoCorrection>
#endif
#if HAVE_TEXT_AUTOGENERATE_TEXT
#include <TextAutoGenerateText/TextAutoGenerateManager>
#endif
static Ruqola *s_self = nullptr;

using namespace Qt::Literals::StringLiterals;
Ruqola::Ruqola(QObject *parent)
    : QObject(parent)
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    , mAutoCorrection(new TextAutoCorrectionCore::AutoCorrection())
#endif
#if HAVE_TEXT_AUTOGENERATE_TEXT
    , mManager(new TextAutoGenerateText::TextAutoGenerateManager(this))
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

static bool sUseRestApiLogin = true;

bool Ruqola::useRestApiLogin()
{
    return sUseRestApiLogin;
}

void Ruqola::setUseRestApiLogin(bool newUseRestApiLogin)
{
    sUseRestApiLogin = newUseRestApiLogin;
}

QWidget *Ruqola::parentWidget() const
{
    return mParentWidget;
}

void Ruqola::setParentWidget(QWidget *newParentWidget)
{
    mParentWidget = newParentWidget;
}
#if HAVE_TEXT_AUTOGENERATE_TEXT
TextAutoGenerateText::TextAutoGenerateManager *Ruqola::textAutoGenerateManager() const
{
    return mManager;
}
#endif

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
