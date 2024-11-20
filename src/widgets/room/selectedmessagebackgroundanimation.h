/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QColor>
#include <QObject>
#include <QPersistentModelIndex>
class MessagesModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SelectedMessageBackgroundAnimation : public QObject
{
    Q_OBJECT
public:
    explicit SelectedMessageBackgroundAnimation(MessagesModel *model, QObject *parent = nullptr);
    ~SelectedMessageBackgroundAnimation() override;

    [[nodiscard]] QColor backgroundColor() const;
    void setBackgroundColor(const QColor &newBackgroundColor);

    void start();

    [[nodiscard]] MessagesModel *messageModel() const;

    [[nodiscard]] QPersistentModelIndex modelIndex() const;
    void setModelIndex(const QPersistentModelIndex &newModelIndex);

Q_SIGNALS:
    void backgroundColorChanged();
    void animationFinished();

private:
    QColor m_backgroundColor;
    QPersistentModelIndex mModelIndex;
    MessagesModel *const mModel;
};
