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

#include "reconnectinfowidget.h"
#include <KLocalizedString>
#include <QTimer>
#include <chrono>
using namespace std::chrono_literals;

ReconnectInfoWidget::ReconnectInfoWidget(QWidget *parent)
    : KMessageWidget(parent)
    , mDelayTimer(new QTimer(this))
{
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Information);
    mDelayTimer->setSingleShot(true);
    mDelayTimer->setInterval(1s);
    connect(mDelayTimer, &QTimer::timeout, this, &ReconnectInfoWidget::slotUpdateTimer);

    connect(this, &KMessageWidget::linkActivated, this, &ReconnectInfoWidget::slotLinkActivated);
}

ReconnectInfoWidget::~ReconnectInfoWidget()
{
}

void ReconnectInfoWidget::slotUpdateTimer()
{
    mCurrentDelay--;
    if (mCurrentDelay == 0) {
        updateText();
        Q_EMIT tryReconnect();
    } else {
        updateText();
        mDelayTimer->start();
    }
}

void ReconnectInfoWidget::slotLinkActivated(const QString &contents)
{
    if (contents == QLatin1String("try_reconnect")) {
        if (mDelayTimer->isActive()) {
            mDelayTimer->stop();
        }
        Q_EMIT tryReconnect();
    }
}

int ReconnectInfoWidget::reconnectSecondDelay() const
{
    return mReconnectSecondDelay;
}

void ReconnectInfoWidget::setReconnectSecondDelay(int newReconnectDelay)
{
    mReconnectSecondDelay = newReconnectDelay;
    mCurrentDelay = mReconnectSecondDelay;
    updateText();
    animatedShow();
    if (mDelayTimer->isActive()) {
        mDelayTimer->stop();
    }
    mDelayTimer->start();
}

void ReconnectInfoWidget::updateText()
{
    setText(i18n("%1 seconds before reconnecting. %2", mCurrentDelay, QStringLiteral("<a href=\"try_reconnect\">%1</a>").arg(i18n("(Try Reconnect)"))));
}
