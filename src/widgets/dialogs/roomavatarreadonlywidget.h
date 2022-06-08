/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QLabel>

class LIBRUQOLAWIDGETS_TESTS_EXPORT RoomAvatarReadOnlyWidget : public QLabel
{
    Q_OBJECT
public:
    explicit RoomAvatarReadOnlyWidget(QWidget *parent = nullptr);
    ~RoomAvatarReadOnlyWidget() override;

    void setCurrentIconPath(const QString &currentPath);

};
