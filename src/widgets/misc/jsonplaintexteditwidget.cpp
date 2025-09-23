/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "jsonplaintexteditwidget.h"
#include "ruqolawidgets_debug.h"
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Theme>
#include <QVBoxLayout>
#include <TextCustomEditor/PlainTextEditor>
#include <TextCustomEditor/PlainTextEditorWidget>
using namespace Qt::Literals::StringLiterals;

JsonPlainTextEditor::JsonPlainTextEditor(QWidget *parent)
    : TextCustomEditor::PlainTextEditor(parent)
{
    setReadOnly(true);
    setSearchSupport(true);
}

JsonPlainTextEditor::~JsonPlainTextEditor() = default;

JsonPlainTextEditWidget::JsonPlainTextEditWidget(QWidget *parent)
    : QWidget{parent}
    , mPlainTextEdit(new TextCustomEditor::PlainTextEditorWidget(new JsonPlainTextEditor(parent), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mPlainTextEdit->setObjectName(u"mPlainTextEdit"_s);
    mPlainTextEdit->setReadOnly(true);
    mainLayout->addWidget(mPlainTextEdit);

    const KSyntaxHighlighting::Definition def = mRepo.definitionForName(u"Json"_s);
    if (!def.isValid()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Invalid definition name";
    }

    auto hl = new KSyntaxHighlighting::SyntaxHighlighter(mPlainTextEdit->editor()->document());
    hl->setTheme((palette().color(QPalette::Base).lightness() < 128) ? mRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                                                                     : mRepo.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
    hl->setDefinition(def);
}

JsonPlainTextEditWidget::~JsonPlainTextEditWidget() = default;

void JsonPlainTextEditWidget::setPlainText(const QString &str)
{
    mPlainTextEdit->setPlainText(str);
}

#include "moc_jsonplaintexteditwidget.cpp"
