/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <KSyntaxHighlighting/Repository>
#include <TextCustomEditor/PlainTextEditor>

namespace TextCustomEditor
{
class PlainTextEditorWidget;
}
class JsonPlainTextEditor : public TextCustomEditor::PlainTextEditor
{
    Q_OBJECT
public:
    explicit JsonPlainTextEditor(QWidget *parent = nullptr);
    ~JsonPlainTextEditor() override;
};

class JsonPlainTextEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit JsonPlainTextEditWidget(QWidget *parent = nullptr);
    ~JsonPlainTextEditWidget() override;

    void setPlainText(const QString &str);

private:
    TextCustomEditor::PlainTextEditorWidget *const mPlainTextEdit;
    KSyntaxHighlighting::Repository mRepo;
};
