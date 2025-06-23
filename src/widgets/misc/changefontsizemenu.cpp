/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changefontsizemenu.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqolaglobalconfig.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QGuiApplication>
#include <QMenu>

ChangeFontSizeMenu::ChangeFontSizeMenu(QObject *parent)
    : KActionMenu{parent}
{
    setText(i18n("Font Size"));

    auto increaseFontSizeAction = new QAction(QIcon::fromTheme(u"zoom-in"_s), i18nc("@action", "Increase Font"), this);
    increaseFontSizeAction->setShortcut(Qt::CTRL | Qt::Key_Plus);
    menu()->addAction(increaseFontSizeAction);
    connect(increaseFontSizeAction, &QAction::triggered, this, [this]() {
        changeFontSize(+1);
    });
    auto decreaseFontSizeAction = new QAction(QIcon::fromTheme(u"zoom-out"_s), i18nc("@action", "Decrease Font"), this);
    decreaseFontSizeAction->setShortcut(Qt::CTRL | Qt::Key_Minus);
    menu()->addAction(decreaseFontSizeAction);
    connect(decreaseFontSizeAction, &QAction::triggered, this, [this]() {
        changeFontSize(-1);
    });
}

ChangeFontSizeMenu::~ChangeFontSizeMenu() = default;

void ChangeFontSizeMenu::changeFontSize(int offset)
{
    QFont f;
    if (RuqolaGlobalConfig::self()->useCustomFont()) {
        f = RuqolaGlobalConfig::self()->generalFont();
    } else {
        RuqolaGlobalConfig::self()->setUseCustomFont(true);
        f = qApp->font();
    }
    const int newSize = f.pointSize() + offset;
    if (newSize < 5 || newSize > 30) {
        qCDebug(RUQOLAWIDGETS_LOG) << " We can't use font size < 5 or > 30";
        return;
    }
    f.setPointSize(newSize);
    RuqolaGlobalConfig::self()->setGeneralFont(f);
    RuqolaGlobalConfig::self()->save();
    qApp->setFont(f);
    Q_EMIT fontChanged();
}

#include "moc_changefontsizemenu.cpp"
