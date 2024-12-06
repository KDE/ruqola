/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "otrwidget.h"

#include <KLocalizedString>
#include <QAction>

OtrWidget::OtrWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Information);
    setPosition(KMessageWidget::Header);
    setText(i18n("Otr Activated"));
    setTextFormat(Qt::PlainText);
    auto endOtrAction = new QAction(i18nc("@action", "End OTR"), this);
    endOtrAction->setObjectName(QStringLiteral("endOtrAction"));
    connect(endOtrAction, &QAction::triggered, this, &OtrWidget::closeOtr);
    addAction(endOtrAction);

    auto refreshKeysAction = new QAction(i18nc("@action", "Refresh Keys"), this);
    refreshKeysAction->setObjectName(QStringLiteral("refreshKeysAction"));
    connect(refreshKeysAction, &QAction::triggered, this, &OtrWidget::refreshKeys);
    addAction(refreshKeysAction);
}

OtrWidget::~OtrWidget() = default;

#include "moc_otrwidget.cpp"
