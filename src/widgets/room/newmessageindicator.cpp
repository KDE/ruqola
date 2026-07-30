/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "newmessageindicator.h"
#include "ruqola_newmessageindicator_debug.h"
#include <KLocalizedString>
#include <QMouseEvent>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
NewMessageIndicator::NewMessageIndicator(QWidget *parent)
    : QWidget{parent}
    , mMessageWidget(new NewMessageIndicatorWidget(this))
{
    setObjectName(u"NewMessageIndicator"_s);
    setFocusPolicy(Qt::NoFocus);
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(u"mainLayout"_s);

    mMessageWidget->setObjectName(u"messageWidget"_s);
    mMessageWidget->setCloseButtonVisible(false);

    mainLayout->addWidget(mMessageWidget);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    mMessageWidget->show();
    mMessageWidget->setText(i18n("New Message"));
    mMessageWidget->setWordWrap(false);

    mMessageWidget->setMessageType(KMessageWidget::Information);
    connect(mMessageWidget, &NewMessageIndicatorWidget::clicked, this, &NewMessageIndicator::moveToBottom);
}

NewMessageIndicator::~NewMessageIndicator() = default;

void NewMessageIndicator::showNewMessageIndicator(bool visible)
{
    qCDebug(RUQOLA_NEWMESSAGEINDICATOR_WIDGETS_LOG) << "NewMessageIndicator::showNewMessageIndicator: " << visible;
    setVisible(visible);
}

NewMessageIndicatorWidget::NewMessageIndicatorWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setMouseTracking(true);
}

NewMessageIndicatorWidget::~NewMessageIndicatorWidget() = default;

void NewMessageIndicatorWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        Q_EMIT clicked();
    }
    QFrame::mousePressEvent(event);
}

#include "moc_newmessageindicator.cpp"
