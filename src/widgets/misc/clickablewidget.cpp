/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "clickablewidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>

#include <QHBoxLayout>
#include <QIcon>

ClickableWidget::ClickableWidget(const QString &userName, QWidget *parent)
    : QWidget(parent)
    , mName(userName)
    , mLabel(new QLabel(mName, this))
    , mClickableLabel(new ClickableLabel(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mLabel->setObjectName(u"mUserLabel"_s);
    mainLayout->addWidget(mLabel);

    mClickableLabel->setObjectName(u"mClickableLabel"_s);
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

QByteArray ClickableWidget::identifier() const
{
    return mIdentifier;
}

void ClickableWidget::setIdentifier(const QByteArray &userId)
{
    mIdentifier = userId;
}

ClickableLabel::ClickableLabel(QWidget *parent)
    : QLabel(parent)
{
    setToolTip(i18nc("@info:tooltip", "Remove"));
    setPixmap(QIcon::fromTheme(u"delete"_s).pixmap(18, 18));
}

ClickableLabel::~ClickableLabel() = default;

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    Q_EMIT clicked();
    QLabel::mousePressEvent(event);
}

#include "moc_clickablewidget.cpp"
