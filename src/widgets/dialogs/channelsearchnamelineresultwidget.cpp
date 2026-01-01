/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelsearchnamelineresultwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

ChannelSearchNameLineResultWidget::ChannelSearchNameLineResultWidget(QWidget *parent)
    : QWidget(parent)
    , mLabel(new QLabel(this))
    , mClearToolButton(new QToolButton(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mLabel->setObjectName(u"mLabel"_s);
    mainLayout->addWidget(mLabel);

    mClearToolButton->setObjectName(u"mClearToolButton"_s);
    mClearToolButton->setIcon(QIcon::fromTheme(u"delete"_s));
    mClearToolButton->setToolTip(i18nc("@info:tooltip", "Clear"));
    mainLayout->addWidget(mClearToolButton);
    connect(mClearToolButton, &QToolButton::clicked, this, &ChannelSearchNameLineResultWidget::clearRoomName);
}

ChannelSearchNameLineResultWidget::~ChannelSearchNameLineResultWidget() = default;

void ChannelSearchNameLineResultWidget::setRoomName(const QString &name)
{
    mLabel->setText(name);
}

void ChannelSearchNameLineResultWidget::setReadOnly(bool readOnly)
{
    mClearToolButton->setVisible(!readOnly);
}

#include "moc_channelsearchnamelineresultwidget.cpp"
