/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QAbstractListModel>

class PreviewCommandModel : public QAbstractListModel
{
public:
    explicit PreviewCommandModel(QObject *parent = nullptr);
    ~PreviewCommandModel() override;
};
