/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "selectedmessagebackgroundanimation.h"

#include <QPropertyAnimation>

SelectedMessageBackgroundAnimation::SelectedMessageBackgroundAnimation(MessageModel *model, QObject *parent)
    : QObject{parent}
    , mModel(model)
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
    if (mModel) {
        auto animation = new QPropertyAnimation(this, "backgroundColor", this);
        animation->setDuration(2000);
        animation->setStartValue(QColor(Qt::red));
        animation->setEndValue(QColor(Qt::transparent));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start();
        connect(animation, &QPropertyAnimation::finished, this, &SelectedMessageBackgroundAnimation::deleteLater);
    } else {
        deleteLater();
    }
}

MessageModel *SelectedMessageBackgroundAnimation::messageModel() const
{
    return mModel;
}
