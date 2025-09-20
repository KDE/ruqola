/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractiongui.h"

#include "autogenerateui/autogenerateinteractionui.h"
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionGui::AutoGenerateInteractionGui(QWidget *parent)
    : QWidget(parent)
    , mPlainTextEdit(new QPlainTextEdit(this))
    , mSerializeTextEdit(new QPlainTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mPlainTextEdit);

    auto button = new QPushButton(u"Generate"_s, this);
    mainLayout->addWidget(button);
    auto engine = new AutoGenerateInteractionUi(nullptr);
    connect(button, &QPushButton::clicked, this, [this, engine]() {
        const QString json = mPlainTextEdit->toPlainText();
        if (!json.isEmpty()) {
            const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
            if (engine->parseInteractionUi(doc.object())) {
                auto w = engine->generateWidget(nullptr);
                w->show();
            } else {
                qWarning() << "Invalid Json" << json;
            }
        }
    });

    mSerializeTextEdit->setReadOnly(true);
    mainLayout->addWidget(mSerializeTextEdit);
    auto generateJsonbutton = new QPushButton(u"Generate Json"_s, this);
    mainLayout->addWidget(generateJsonbutton);
    connect(generateJsonbutton, &QPushButton::clicked, this, [this, engine]() {
        mSerializeTextEdit->clear();
        const QJsonObject o = engine->generateJson(true); // Generate state!
        const QJsonDocument d(o);
        const QByteArray ba = d.toJson();
        mSerializeTextEdit->setPlainText(QString::fromUtf8(ba));
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
