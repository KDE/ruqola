/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "selectedmessagebackgroundanimation.h"

#include <QPropertyAnimation>

SelectedMessageBackgroundAnimation::SelectedMessageBackgroundAnimation(QObject *parent)
    : QObject{parent}
{
}

SelectedMessageBackgroundAnimation::~SelectedMessageBackgroundAnimation() = default;

QColor SelectedMessageBackgroundAnimation::backgroundColor() const
{
    return m_backgroundColor;
}

void SelectedMessageBackgroundAnimation::setBackgroundColor(const QColor &newBackgroundColor)
{
    if (m_backgroundColor == newBackgroundColor)
        return;
    m_backgroundColor = newBackgroundColor;
    Q_EMIT backgroundColorChanged();
}

void SelectedMessageBackgroundAnimation::start()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "backgroundColor");
    animation->setDuration(2000);
    animation->setStartValue(QColor(0, 0, 0));
    animation->setEndValue(QColor(Qt::transparent));
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, &SelectedMessageBackgroundAnimation::deleteLater);
}
