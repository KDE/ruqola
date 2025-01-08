/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 * SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include "config-ruqola.h"
#include "libruqolacore_export.h"
#include <QObject>

class RocketChatAccount;
class AccountManager;
namespace TextAutoCorrection
{
class AutoCorrection;
}
namespace TextAutoCorrectionCore
{
class AutoCorrection;
}
class CustomEmojiIconManager;
class LIBRUQOLACORE_EXPORT Ruqola : public QObject
{
    Q_OBJECT
public:
    ~Ruqola() override;
    /**
     * @brief Singleton provider
     *
     * @return Returns the singleton object
     */
    static Ruqola *self();

    static void destroy();
    RocketChatAccount *rocketChatAccount() const;

    AccountManager *accountManager() const;

    explicit Ruqola(QObject *parent = nullptr);

    void setCurrentAccount(const QString &accountName);

#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    TextAutoCorrectionCore::AutoCorrection *autoCorrection() const;
#endif

    CustomEmojiIconManager *customEmojiIconManager() const;

    [[nodiscard]] bool debug() const;

    [[nodiscard]] static bool useRestApiLogin();
    static void setUseRestApiLogin(bool newUseRestApiLogin);

Q_SIGNALS:
    void translatorMenuChanged();

private:
    AccountManager *mAccountManager = nullptr;
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    TextAutoCorrectionCore::AutoCorrection *const mAutoCorrection = nullptr;
#endif

    CustomEmojiIconManager *const mCustomEmojiIconManager;
    bool mDebug = false;
};
