/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

    auto endOtrAction = new QAction(i18n("End OTR"), this);
    endOtrAction->setObjectName(QStringLiteral("endOtrAction"));
    connect(endOtrAction, &QAction::triggered, this, &OtrWidget::slotCloseOtr);

    auto refreshKeysAction = new QAction(i18n("Refresh Keys"), this);
    refreshKeysAction->setObjectName(QStringLiteral("refreshKeysAction"));
    connect(refreshKeysAction, &QAction::triggered, this, &OtrWidget::slotRefreshKeys);
}

OtrWidget::~OtrWidget()
{
}

void OtrWidget::slotCloseOtr()
{
    // Need Otr here.
    // TODO missing roomId/userId I think
    // mRocketChatAccount->ddp()->streamNotifyUserOtrEnd(...);
    Q_EMIT closeOtr();
}

void OtrWidget::slotRefreshKeys()
{
    // Need Otr here.
    // TODO missing roomId/userId I think
    Q_EMIT refreshKeys();
}
