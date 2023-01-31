/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QColor>
#include <QObject>

class SelectedMessageBackgroundAnimation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
public:
    explicit SelectedMessageBackgroundAnimation(QObject *parent = nullptr);
    ~SelectedMessageBackgroundAnimation() override;

    Q_REQUIRED_RESULT QColor backgroundColor() const;
    void setBackgroundColor(const QColor &newBackgroundColor);

Q_SIGNALS:
    void backgroundColorChanged();

private:
    QColor m_backgroundColor;
};
