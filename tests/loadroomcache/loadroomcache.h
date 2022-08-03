/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class QTextEdit;
class KUrlRequester;
class LoadRoomCache : public QWidget
{
    Q_OBJECT
public:
    explicit LoadRoomCache(QWidget *parent = nullptr);
    ~LoadRoomCache() override = default;

private:
    void slotOpenFile();
    QTextEdit *const mCacheTextEdit;
    KUrlRequester *const mRequester;
};
