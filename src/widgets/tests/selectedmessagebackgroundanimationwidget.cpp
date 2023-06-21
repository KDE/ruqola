/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "selectedmessagebackgroundanimationwidget.h"
#include "room/selectedmessagebackgroundanimation.h"
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
SelectedMessageBackgroundAnimationWidget::SelectedMessageBackgroundAnimationWidget(QWidget *parent)
    : QWidget{parent}
    , mLabel(new SelectedMessageBackgroundAnimationLabel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mLabel);

    auto pushButton = new QPushButton(QStringLiteral("Activate"), this);
    mainLayout->addWidget(pushButton);
    connect(pushButton, &QPushButton::clicked, this, [this] {
        auto animation = new SelectedMessageBackgroundAnimation(nullptr, this);
        connect(animation, &SelectedMessageBackgroundAnimation::backgroundColorChanged, this, [this, animation]() {
            mLabel->setBackgroundColor(animation->backgroundColor());
        });
        animation->start();
    });
}

SelectedMessageBackgroundAnimationWidget::~SelectedMessageBackgroundAnimationWidget() = default;

// Label
SelectedMessageBackgroundAnimationLabel::SelectedMessageBackgroundAnimationLabel(QWidget *parent)
    : QWidget(parent)
{
}

SelectedMessageBackgroundAnimationLabel::~SelectedMessageBackgroundAnimationLabel() = default;

void SelectedMessageBackgroundAnimationLabel::setBackgroundColor(const QColor &col)
{
    mBackgroundColor = col;
    update();
}

void SelectedMessageBackgroundAnimationLabel::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.fillRect(event->rect(), mBackgroundColor);
}

#include "moc_selectedmessagebackgroundanimationwidget.cpp"
