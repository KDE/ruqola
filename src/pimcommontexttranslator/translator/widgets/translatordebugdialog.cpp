/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translatordebugdialog.h"
#include <QPlainTextEdit>

#include <KConfigGroup>
#include <KLocalizedString>
#include <KMessageBox>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QPointer>
#include <QPushButton>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
const char myTranslatorDebugDialogConfigGroupName[] = "TranslatorDebugDialog";
}
using namespace PimCommonTextTranslator;
TranslatorDebugDialog::TranslatorDebugDialog(QWidget *parent)
    : QDialog(parent)
    , mEdit(new QPlainTextEdit(this))
    , mUser1Button(new QPushButton(this))
{
    setWindowTitle(i18nc("@title:window", "Translator Debug"));
    auto mainLayout = new QVBoxLayout(this);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->addButton(mUser1Button, QDialogButtonBox::ActionRole);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &TranslatorDebugDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &TranslatorDebugDialog::reject);
    mUser1Button->setText(i18n("Save As..."));
    connect(mUser1Button, &QPushButton::clicked, this, &TranslatorDebugDialog::slotSaveAs);

    mEdit->setReadOnly(true);
    mainLayout->addWidget(mEdit);
    mainLayout->addWidget(buttonBox);

    readConfig();
    mUser1Button->setEnabled(!mEdit->toPlainText().isEmpty());
}

TranslatorDebugDialog::~TranslatorDebugDialog()
{
    writeConfig();
}

void TranslatorDebugDialog::setDebug(const QString &debugStr)
{
    mEdit->setPlainText(debugStr);
    mUser1Button->setEnabled(!debugStr.isEmpty());
}

void TranslatorDebugDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), myTranslatorDebugDialogConfigGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void TranslatorDebugDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myTranslatorDebugDialogConfigGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

void TranslatorDebugDialog::saveTextAs(const QString &text, const QString &filter, QWidget *parent, const QUrl &url, const QString &caption)
{
    QPointer<QFileDialog> fdlg(new QFileDialog(parent, QString(), url.path(), filter));
    if (!caption.isEmpty()) {
        fdlg->setWindowTitle(caption);
    }
    fdlg->setAcceptMode(QFileDialog::AcceptSave);
    if (fdlg->exec() == QDialog::Accepted) {
        const QString fileName = fdlg->selectedFiles().at(0);
        if (!saveToFile(fileName, text)) {
            KMessageBox::error(parent,
                               i18n("Could not write the file %1:\n"
                                    "\"%2\" is the detailed error description.",
                                    fileName,
                                    QString::fromLocal8Bit(strerror(errno))),
                               i18n("Save File Error"));
        }
    }
    delete fdlg;
}

bool TranslatorDebugDialog::saveToFile(const QString &filename, const QString &text)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream out(&file);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    out.setCodec("UTF-8");
#endif
    out << text;
    file.close();
    return true;
}

void TranslatorDebugDialog::slotSaveAs()
{
    const QString filter = i18n("All Files (*)");
    saveTextAs(mEdit->toPlainText(), filter, this);
}
