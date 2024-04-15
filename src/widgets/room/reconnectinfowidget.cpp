/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reconnectinfowidget.h"
using namespace Qt::Literals::StringLiterals;

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
    setPosition(KMessageWidget::Header);
    mDelayTimer->setSingleShot(true);
    mDelayTimer->setInterval(1s);
    connect(mDelayTimer, &QTimer::timeout, this, &ReconnectInfoWidget::slotUpdateTimer);

    connect(this, &KMessageWidget::linkActivated, this, &ReconnectInfoWidget::slotLinkActivated);
}

ReconnectInfoWidget::~ReconnectInfoWidget() = default;

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
    if (contents == "try_reconnect"_L1) {
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

#include "moc_reconnectinfowidget.cpp"
