/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QColor>
#include <QListView>
class SearchChannelListView : public QListView
{
    Q_OBJECT
public:
    explicit SearchChannelListView(QWidget *parent = nullptr);
    ~SearchChannelListView() override;

    void setSearchChannel(bool search);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void generalPaletteChanged();
    QColor mTextColor;
    bool mSearchChannel = false;
};
