/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDialog>
#include <QUrl>

class QPlainTextEdit;
namespace TextTranslator
{
class TranslatorDebugDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TranslatorDebugDialog(QWidget *parent = nullptr);
    ~TranslatorDebugDialog() override;

    void setDebug(const QString &debugStr);

private:
    void slotSaveAs();
    void readConfig();
    void saveTextAs(const QString &text, const QString &filter, QWidget *parent, const QUrl &url = {}, const QString &caption = {});
    Q_REQUIRED_RESULT bool saveToFile(const QString &filename, const QString &text);
    void writeConfig();
    QPlainTextEdit *const mEdit;
    QPushButton *const mUser1Button;
};
}
