/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "clickablewidget.h"
#include <QHBoxLayout>
#include <QIcon>

ClickableWidget::ClickableWidget(const QString &userName, QWidget *parent)
    : QWidget(parent)
    , mName(userName)
    , mLabel(new QLabel(mName, this))
    , mClickableLabel(new ClickableLabel(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mLabel->setObjectName(QStringLiteral("mUserLabel"));
    mainLayout->addWidget(mLabel);

    mClickableLabel->setObjectName(QStringLiteral("mClickableLabel"));
    mainLayout->addWidget(mClickableLabel);
    connect(mClickableLabel, &ClickableLabel::clicked, this, &ClickableWidget::slotRemove);
}

ClickableWidget::~ClickableWidget() = default;

void ClickableWidget::slotRemove()
{
    Q_EMIT removeClickableWidget(mName);
}

QString ClickableWidget::name() const
{
    return mName;
}

void ClickableWidget::setName(const QString &userName)
{
    mName = userName;
}

QString ClickableWidget::identifier() const
{
    return mIdentifier;
}

void ClickableWidget::setIdentifier(const QString &userId)
{
    mIdentifier = userId;
}

ClickableLabel::ClickableLabel(QWidget *parent)
    : QLabel(parent)
{
    setPixmap(QIcon::fromTheme(QStringLiteral("delete")).pixmap(18, 18));
}

ClickableLabel::~ClickableLabel() = default;

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    Q_EMIT clicked();
    QLabel::mousePressEvent(event);
}

#include "moc_clickablewidget.cpp"
