/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>
  based on code from kopete

  SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "emoticontexteditaction.h"
#include "emoticontexteditselector.h"

#include <KLocalizedString>
#include <QIcon>
#include <QMenu>

#include <QWidgetAction>

using namespace TextEmoticonsWidgets;

class EmoticonTextEditAction::EmoticonTextEditActionPrivate
{
public:
    EmoticonTextEditActionPrivate()
        : emoticonMenu(new QMenu())
    {
        selector = new EmoticonTextEditSelector(emoticonMenu);
        auto action = new QWidgetAction(emoticonMenu);
        action->setDefaultWidget(selector);
        emoticonMenu->addAction(action);
        connect(emoticonMenu, &QMenu::aboutToShow, selector, &EmoticonTextEditSelector::loadEmoticons);
    }

    ~EmoticonTextEditActionPrivate()
    {
        delete emoticonMenu;
    }

    void setCustomEmojiSupport(bool b);
    Q_REQUIRED_RESULT bool customEmojiSupport() const;

    QMenu *const emoticonMenu;
    EmoticonTextEditSelector *selector = nullptr;
};

void EmoticonTextEditAction::EmoticonTextEditActionPrivate::setCustomEmojiSupport(bool b)
{
    selector->setCustomEmojiSupport(b);
}

bool EmoticonTextEditAction::EmoticonTextEditActionPrivate::customEmojiSupport() const
{
    return selector->customEmojiSupport();
}

EmoticonTextEditAction::EmoticonTextEditAction(QObject *parent)
    : KActionMenu(i18n("Add Smiley"), parent)
    , d(new EmoticonTextEditActionPrivate())
{
    delete menu();
    setMenu(d->emoticonMenu);
    setIcon(QIcon::fromTheme(QStringLiteral("face-smile")));
    setPopupMode(QToolButton::InstantPopup);
    connect(d->selector, &EmoticonTextEditSelector::insertEmoji, this, &EmoticonTextEditAction::insertEmoticon);
}

EmoticonTextEditAction::~EmoticonTextEditAction() = default;

void EmoticonTextEditAction::setCustomEmojiSupport(bool b)
{
    d->setCustomEmojiSupport(b);
}

bool EmoticonTextEditAction::customEmojiSupport() const
{
    return d->customEmojiSupport();
}

#include "moc_emoticontexteditaction.cpp"
