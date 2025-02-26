/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractiongui.h"

#include "autogenerateui/autogenerateinteractionui.h"
#include <QApplication>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QStandardPaths>

AutoGenerateInteractionGui::AutoGenerateInteractionGui(QWidget *parent)
    : QWidget(parent)
    , mPlainTextEdit(new QPlainTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mPlainTextEdit);

    auto button = new QPushButton(QStringLiteral("Generate"), this);
    mainLayout->addWidget(button);
    connect(button, &QPushButton::clicked, this, [this]() {
        const QString json = mPlainTextEdit->toPlainText();
        if (!json.isEmpty()) {
            AutoGenerateInteractionUi engine;
            const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
            if (engine.parseInteractionUi(doc.object())) {
                auto w = engine.generateWidget(nullptr);
                w->show();
            } else {
                qWarning() << "Invalid Json" << json;
            }
        }
    });
    resize(800, 600);
}

AutoGenerateInteractionGui::~AutoGenerateInteractionGui() = default;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);

    AutoGenerateInteractionGui w;
    w.show();
    return app.exec();
}

#include "moc_autogenerateinteractiongui.cpp"
