/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QColor>
#include <QObject>
class MessageModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SelectedMessageBackgroundAnimation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
public:
    explicit SelectedMessageBackgroundAnimation(MessageModel *model, QObject *parent = nullptr);
    ~SelectedMessageBackgroundAnimation() override;

    Q_REQUIRED_RESULT QColor backgroundColor() const;
    void setBackgroundColor(const QColor &newBackgroundColor);

    void start();

    Q_REQUIRED_RESULT MessageModel *messageModel() const;

Q_SIGNALS:
    void backgroundColorChanged();

private:
    QColor m_backgroundColor;
    MessageModel *const mModel;
};
