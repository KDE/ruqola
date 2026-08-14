/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "selectedmessagebackgroundanimation.h"

#include "colorsandmessageviewstyle.h"
#include "ruqolawidgets_debug.h"
#include <QPropertyAnimation>
using namespace Qt::Literals::StringLiterals;

SelectedMessageBackgroundAnimation::SelectedMessageBackgroundAnimation(QObject *parent)
    : QObject{parent}
{
}

SelectedMessageBackgroundAnimation::~SelectedMessageBackgroundAnimation() = default;

QColor SelectedMessageBackgroundAnimation::backgroundColor() const
{
    return m_backgroundColor;
}

QPersistentModelIndex SelectedMessageBackgroundAnimation::modelIndex() const
{
    return mModelIndex;
}

void SelectedMessageBackgroundAnimation::setModelIndex(const QPersistentModelIndex &newModelIndex)
{
    mModelIndex = newModelIndex;
}

void SelectedMessageBackgroundAnimation::setBackgroundColor(const QColor &newBackgroundColor)
{
    if (m_backgroundColor == newBackgroundColor)
        return;
    m_backgroundColor = newBackgroundColor;
    qCDebug(RUQOLAWIDGETS_LOG) << " Background color changed " << newBackgroundColor;
    Q_EMIT backgroundColorChanged();
}

void SelectedMessageBackgroundAnimation::start()
{
    auto animation = new QPropertyAnimation(this, "backgroundColor"_ba, this);
    animation->setDuration(2000);
    const auto color = ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::NeutralText).color();
    // Fade out the alpha channel only: interpolating towards Qt::transparent (transparent black)
    // would drag the rgb channels to black and darken the highlight instead of dissolving it.
    QColor endColor = color;
    endColor.setAlpha(0);
    animation->setStartValue(color);
    animation->setEndValue(endColor);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        Q_EMIT animationFinished();
        deleteLater();
    });
}

#include "moc_selectedmessagebackgroundanimation.cpp"
