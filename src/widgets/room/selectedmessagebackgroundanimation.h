/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QColor>
#include <QObject>
#include <QPersistentModelIndex>
class MessageModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SelectedMessageBackgroundAnimation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
public:
    explicit SelectedMessageBackgroundAnimation(MessageModel *model, QObject *parent = nullptr);
    ~SelectedMessageBackgroundAnimation() override;

    [[nodiscard]] QColor backgroundColor() const;
    void setBackgroundColor(const QColor &newBackgroundColor);

    void start();

    [[nodiscard]] MessageModel *messageModel() const;

    [[nodiscard]] QPersistentModelIndex modelIndex() const;
    void setModelIndex(const QPersistentModelIndex &newModelIndex);

Q_SIGNALS:
    void backgroundColorChanged();

private:
    void slotBackgroundColorChanged();
    QColor m_backgroundColor;
    QPersistentModelIndex mModelIndex;
    MessageModel *const mModel;
};
