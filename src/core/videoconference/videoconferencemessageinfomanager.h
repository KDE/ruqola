/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolacore_export.h"
#include <QObject>

class LIBRUQOLACORE_EXPORT VideoConferenceMessageInfoManager : public QObject
{
    Q_OBJECT
public:
    explicit VideoConferenceMessageInfoManager(QObject *parent = nullptr);
    ~VideoConferenceMessageInfoManager() override;
};
