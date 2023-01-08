/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "autocorrectionwidget_gui.h"
#include "settings/textautocorrectionsettings.h"
#include "widgets/autocorrectionlineedit.h"
#include "widgets/autocorrectiontextedit.h"
#include "widgets/autocorrectionwidget.h"
#include <TextAutoCorrection/AutoCorrection>

#include <QAction>
#include <QApplication>
#include <QCommandLineParser>
#include <QDialogButtonBox>
#include <QPointer>
#include <QPushButton>
#include <QToolBar>
#include <QVBoxLayout>

ConfigureTestDialog::ConfigureTestDialog(TextAutoCorrection::AutoCorrection *autoCorrection, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(QStringLiteral("Configure Autocorrection"));
    auto mainLayout = new QVBoxLayout(this);
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ConfigureTestDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ConfigureTestDialog::reject);

    buttonBox->button(QDialogButtonBox::Ok)->setDefault(true);

    mWidget = new TextAutoCorrection::AutoCorrectionWidget(this);
    mainLayout->addWidget(mWidget);
    mainLayout->addWidget(buttonBox);

    mWidget->setAutoCorrection(autoCorrection);
    mWidget->loadConfig();
    connect(okButton, &QPushButton::clicked, this, &ConfigureTestDialog::slotSaveSettings);
}

ConfigureTestDialog::~ConfigureTestDialog() = default;

void ConfigureTestDialog::slotSaveSettings()
{
    mWidget->writeConfig();
}

AutocorrectionTestWidget::AutocorrectionTestWidget(QWidget *parent)
    : QWidget(parent)
    , mConfig(KSharedConfig::openConfig(QStringLiteral("autocorrectionguirc")))
{
    TextAutoCorrection::TextAutoCorrectionSettings::self()->setSharedConfig(mConfig);
    TextAutoCorrection::TextAutoCorrectionSettings::self()->load();

    mAutoCorrection = new TextAutoCorrection::AutoCorrection;
    auto lay = new QVBoxLayout(this);
    auto bar = new QToolBar;
    lay->addWidget(bar);
    bar->addAction(QStringLiteral("Configure..."), this, &AutocorrectionTestWidget::slotConfigure);
    auto richText = new QAction(QStringLiteral("HTML mode"), this);
    richText->setCheckable(true);
    connect(richText, &QAction::toggled, this, &AutocorrectionTestWidget::slotChangeMode);
    bar->addAction(richText);

    mSubject = new TextAutoCorrection::AutoCorrectionLineEdit(this);
    mSubject->setAutocorrection(mAutoCorrection);
    lay->addWidget(mSubject);

    mEdit = new TextAutoCorrection::AutoCorrectionTextEdit(this);
    mEdit->setAutocorrection(mAutoCorrection);
    lay->addWidget(mEdit);
}

AutocorrectionTestWidget::~AutocorrectionTestWidget()
{
    mAutoCorrection->writeConfig();
    delete mAutoCorrection;
}

void AutocorrectionTestWidget::slotChangeMode(bool mode)
{
    mEdit->setAcceptRichText(mode);
}

void AutocorrectionTestWidget::slotConfigure()
{
    QPointer<ConfigureTestDialog> dlg = new ConfigureTestDialog(mAutoCorrection, this);
    if (dlg->exec()) {
        TextAutoCorrection::TextAutoCorrectionSettings::self()->save();
    }
    delete dlg;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);

    auto w = new AutocorrectionTestWidget();
    w->resize(800, 600);

    w->show();
    const int ret = app.exec();
    delete w;
    return ret;
}
