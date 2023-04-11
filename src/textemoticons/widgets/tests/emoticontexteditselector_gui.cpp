/*
    This file is part of KDE.

    SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>

#include "emoticontexteditselector.h"

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        auto mainLayout = new QVBoxLayout(this);
        auto w = new TextEmoticonsWidgets::EmoticonTextEditSelector(this);
        w->loadEmoticons();

        mainLayout->addWidget(w);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;

    window.show();
    return app.exec();
}

#include "emoticontexteditselector_gui.moc"
