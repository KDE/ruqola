/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "aitextmenuwidget.h"
#include "aitextmanager.h"
#include "aitextmenuconfiguredialog.h"

#include <KLocalizedString>
#include <QMenu>

AiTextMenuWidget::AiTextMenuWidget(QObject *parent)
    : QObject{parent}
    , mAiTextMenu(new QMenu)
    , mAiTextManager(new AiTextManager(this))
{
    mAiTextManager->load();
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
    const auto infos = mAiTextManager->textInfos();
    for (const AiTextInfo &info : infos) {
        if (info.enabled()) {
            auto action = new QAction(info.requestText(), mAiTextMenu);
            connect(action, &QAction::triggered, this, [this]() {
                // TODO
            });
            mAiTextMenu->addAction(action);
        }
    }
    auto configureAction = new QAction(i18nc("@action", "Configure…"), mAiTextMenu);
    connect(configureAction, &QAction::triggered, this, &AiTextMenuWidget::slotConfigure);
    if (!mAiTextMenu->isEmpty()) {
        mAiTextMenu->addSeparator();
    }
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
        mAiTextManager->setTextInfos(dlg->aiTextInfos());
        mAiTextManager->save();
        mAiTextMenu->clear();
        initializeMenu();
    }
    delete dlg;
}

#include "moc_aitextmenuwidget.cpp"
