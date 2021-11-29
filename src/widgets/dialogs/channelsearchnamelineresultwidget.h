/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLabel;
class QToolButton;

class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelSearchNameLineResultWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelSearchNameLineResultWidget(QWidget *parent = nullptr);
    ~ChannelSearchNameLineResultWidget() override;

    void setRoomName(const QString &name);
    void setReadOnly(bool readOnly);

Q_SIGNALS:
    void clearRoomName();

private:
    QLabel *const mLabel;
    QToolButton *const mClearToolButton;
};
