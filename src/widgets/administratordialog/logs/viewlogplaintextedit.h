/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "config-ruqola.h"
#include "libruqolawidgets_private_export.h"
#include <QPlainTextEdit>
#if HAVE_TEXT_CUSTOM_EDITOR
#include <TextCustomEditor/PlainTextEditor>
#include <TextCustomEditor/PlainTextEditorWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ViewLogPlainTextEdit : public TextCustomEditor::PlainTextEditorWidget
{
    Q_OBJECT
public:
    explicit ViewLogPlainTextEdit(QWidget *parent = nullptr);
    ~ViewLogPlainTextEdit() override;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT ViewLogPlainTextEditor : public TextCustomEditor::PlainTextEditor
{
    Q_OBJECT
public:
    explicit ViewLogPlainTextEditor(QWidget *parent = nullptr);
    ~ViewLogPlainTextEditor() override;

protected:
    void addExtraMenuEntry(QMenu *menu, QPoint pos) override;

private:
    void slotSaveAsFile();
    void saveTextAs(const QString &text, const QString &filter, QWidget *parent, const QUrl &url = {}, const QString &caption = {});
    [[nodiscard]] bool saveToFile(const QString &filename, const QString &text);
};
#else
class LIBRUQOLAWIDGETS_TESTS_EXPORT ViewLogPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit ViewLogPlainTextEdit(QWidget *parent = nullptr);
    ~ViewLogPlainTextEdit() override;

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void slotSaveAsFile();
    void saveTextAs(const QString &text, const QString &filter, QWidget *parent, const QUrl &url = {}, const QString &caption = {});
    [[nodiscard]] bool saveToFile(const QString &filename, const QString &text);
};

#endif
