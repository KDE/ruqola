/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "aitextmenuwidget.h"
#include "aitextmenuconfiguredialog.h"

#include <KLocalizedString>
#include <QMenu>

AiTextMenuWidget::AiTextMenuWidget(QObject *parent)
    : QObject{parent}
    , mAiTextMenu(new QMenu)
{
    mAiTextMenu->setTitle(i18n("Ask AI…"));
    // mMenu->setIcon(QIcon::fromTheme(QStringLiteral("document-share")));
    mAiTextMenu->setObjectName(QStringLiteral("mMenu"));
    initializeMenu();
}

AiTextMenuWidget::~AiTextMenuWidget()
{
    delete mAiTextMenu;
}

void AiTextMenuWidget::initializeMenu()
{
    // TODO add list of actions
    auto configureAction = new QAction(i18nc("@action", "Configure…"), mAiTextMenu);
    connect(configureAction, &QAction::triggered, this, &AiTextMenuWidget::slotConfigure);
    mAiTextMenu->addSeparator();
    mAiTextMenu->addAction(configureAction);
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

void AiTextMenuWidget::slotConfigure()
{
    auto dlg = new AiTextMenuConfigureDialog(nullptr);
    if (dlg->exec()) {
        mAiTextMenu->clear();
        initializeMenu();
    }
    delete dlg;
}

#include "moc_aitextmenuwidget.cpp"
