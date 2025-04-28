/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "aitextmenuwidget.h"

#include <KLocalizedString>
#include <QMenu>

AiTextMenuWidget::AiTextMenuWidget(QObject *parent)
    : QObject{parent}
    , mAiTextMenu(new QMenu)
{
    mAiTextMenu->setTitle(i18n("Ask AI…"));
    // mMenu->setIcon(QIcon::fromTheme(QStringLiteral("document-share")));
    mAiTextMenu->setObjectName(QStringLiteral("mMenu"));
    connect(mAiTextMenu, &QMenu::aboutToShow, this, &AiTextMenuWidget::slotInitializeMenu);
}

AiTextMenuWidget::~AiTextMenuWidget()
{
    delete mAiTextMenu;
}

void AiTextMenuWidget::slotInitializeMenu()
{
    // TODO
}

QMenu *AiTextMenuWidget::menu() const
{
    return mAiTextMenu;
}

QString AiTextMenuWidget::selectedText() const
{
    return mSelectedText;
}

void AiTextMenuWidget::setSelectedText(const QString &newSelectedText)
{
    mSelectedText = newSelectedText;
}

#include "moc_aitextmenuwidget.cpp"
