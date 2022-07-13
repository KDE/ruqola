/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "bannermessagewidget.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QAction>

BannerMessageWidget::BannerMessageWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(true);
    setMessageType(Information);
    setWordWrap(true);

    auto readAction = new QAction(i18n("Refresh Keys"), this);
    readAction->setObjectName(QStringLiteral("refreshKeysAction"));
    connect(readAction, &QAction::triggered, this, &BannerMessageWidget::slotReadInfo);
    addAction(readAction);
}

BannerMessageWidget::~BannerMessageWidget() = default;

const QString &BannerMessageWidget::identifier() const
{
    return mIdentifier;
}

void BannerMessageWidget::setIdentifier(const QString &newIdentifier)
{
    mIdentifier = newIdentifier;
}

void BannerMessageWidget::slotReadInfo()
{
    if (mIdentifier.isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << " identifier is empty. It's a bug";
    } else {
    }
}
