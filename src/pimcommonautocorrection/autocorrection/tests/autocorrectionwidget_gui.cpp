/*
  SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "autocorrectionwidget_gui.h"
#include "autocorrection/widgets/autocorrectionlineedit.h"
#include "autocorrection/widgets/autocorrectiontextedit.h"
#include "autocorrection/widgets/autocorrectionwidget.h"
#include "settings/pimcommonautocorrectionsettings.h"
#include <PimCommonAutoCorrection/AutoCorrection>

#include <QAction>
#include <QApplication>
#include <QCommandLineParser>
#include <QDialogButtonBox>
#include <QPointer>
#include <QPushButton>
#include <QToolBar>
#include <QVBoxLayout>

ConfigureTestDialog::ConfigureTestDialog(PimCommonAutoCorrection::AutoCorrection *autoCorrection, QWidget *parent)
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

    mWidget = new PimCommonAutoCorrection::AutoCorrectionWidget(this);
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
    PimCommonAutoCorrection::PimCommonAutoCorrectionSettings::self()->setSharedConfig(mConfig);
    PimCommonAutoCorrection::PimCommonAutoCorrectionSettings::self()->load();

    mAutoCorrection = new PimCommonAutoCorrection::AutoCorrection;
    auto lay = new QVBoxLayout(this);
    auto bar = new QToolBar;
    lay->addWidget(bar);
    bar->addAction(QStringLiteral("Configure..."), this, &AutocorrectionTestWidget::slotConfigure);
    auto richText = new QAction(QStringLiteral("HTML mode"), this);
    richText->setCheckable(true);
    connect(richText, &QAction::toggled, this, &AutocorrectionTestWidget::slotChangeMode);
    bar->addAction(richText);

    mSubject = new PimCommonAutoCorrection::AutoCorrectionLineEdit(this);
    mSubject->setAutocorrection(mAutoCorrection);
    lay->addWidget(mSubject);

    mEdit = new PimCommonAutoCorrection::AutoCorrectionTextEdit(this);
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
        PimCommonAutoCorrection::PimCommonAutoCorrectionSettings::self()->save();
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
