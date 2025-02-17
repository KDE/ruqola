/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "selectedmessagebackgroundanimation.h"
#include "model/messagesmodel.h"

#include "colorsandmessageviewstyle.h"
#include "ruqolawidgets_debug.h"
#include <QPropertyAnimation>

SelectedMessageBackgroundAnimation::SelectedMessageBackgroundAnimation(MessagesModel *model, QObject *parent)
    : QObject{parent}
    , mModel(model)
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
    animation->setStartValue(color);
    animation->setEndValue(QColor(Qt::transparent));
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        Q_EMIT animationFinished();
        deleteLater();
    });
}

MessagesModel *SelectedMessageBackgroundAnimation::messageModel() const
{
    return mModel;
}

#include "moc_selectedmessagebackgroundanimation.cpp"
