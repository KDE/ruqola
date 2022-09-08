/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatormenu.h"
#include "convertertextjob/translatetext/translatorutil.h"
#include "ruqolawidgets_debug.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QMenu>

TranslatorMenu::TranslatorMenu(QObject *parent)
    : QObject(parent)
    , mMenu(new QMenu)
{
    mMenu->setObjectName(QStringLiteral("menu"));
    mMenu->setTitle(i18n("Translate..."));
    mMenu->setIcon(QIcon::fromTheme(QStringLiteral("preferences-desktop-locale")));
    updateMenu();
}

TranslatorMenu::~TranslatorMenu()
{
    delete mMenu;
}

bool TranslatorMenu::isEmpty() const
{
    return mMenu->isEmpty();
}

void TranslatorMenu::updateMenu()
{
    mMenu->clear();
    KConfigGroup groupTranslate(KSharedConfig::openConfig(), QStringLiteral("Translate"));
    const QString engine = groupTranslate.readEntry(QStringLiteral("engine"), QStringLiteral("google")); // Google by default
    const auto fromList = groupTranslate.readEntry(QStringLiteral("From"), QStringList());
    const auto toList = groupTranslate.readEntry(QStringLiteral("To"), QStringList());
    const QVector<QPair<QString, QString>> languagesList = TranslatorUtil::supportedLanguages(engine);
    for (const auto &fromLang : fromList) {
        const QString fromLangI18n = searchI18nFromLanguage(languagesList, fromLang);
        if (fromLangI18n.isEmpty()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to find \"from\" language " << fromLangI18n;
        } else {
            for (const auto &toLang : toList) {
                const QString toLangI18n = searchI18nFromLanguage(languagesList, toLang);
                if (toLangI18n.isEmpty()) {
                    qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to find \"to\" language " << fromLangI18n;
                } else {
                    auto action = new QAction(mMenu);
                    action->setText(QStringLiteral("%1->%2").arg(fromLangI18n, toLangI18n));
                    connect(action, &QAction::triggered, this, [this, fromLang, toLang]() {
                        Q_EMIT translate(fromLang, toLang, mMessageId);
                    });
                    mMenu->addAction(action);
                }
            }
        }
    }
}

const QString &TranslatorMenu::messageId() const
{
    return mMessageId;
}

void TranslatorMenu::setMessageId(const QString &newMessageId)
{
    mMessageId = newMessageId;
}

QString TranslatorMenu::searchI18nFromLanguage(const QVector<QPair<QString, QString>> &languagesList, const QString &lang)
{
    for (const QPair<QString, QString> &pair : languagesList) {
        if (pair.second == lang) {
            return pair.first;
        }
    }
    return {};
}

QMenu *TranslatorMenu::menu() const
{
    return mMenu;
}
