/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <KSyntaxHighlighting/Repository>
#include <QWidget>
#include <TextCustomEditor/PlainTextEditor>

namespace TextCustomEditor
{
class PlainTextEditorWidget;
}
class ExploreDatabasePlainTextEditor : public TextCustomEditor::PlainTextEditor
{
    Q_OBJECT
public:
    explicit ExploreDatabasePlainTextEditor(QWidget *parent = nullptr);
    ~ExploreDatabasePlainTextEditor() override;
};

class ExploreDatabaseJsonPlainTextEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseJsonPlainTextEditWidget(QWidget *parent = nullptr);
    ~ExploreDatabaseJsonPlainTextEditWidget() override;

    void setPlainText(const QString &str);

private:
    TextCustomEditor::PlainTextEditorWidget *const mPlainTextEdit;
    KSyntaxHighlighting::Repository mRepo;
};
