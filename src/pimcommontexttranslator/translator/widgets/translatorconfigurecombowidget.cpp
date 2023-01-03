/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatorconfigurecombowidget.h"
#include "translator/misc/translatorutil.h"
#include "translator/translatorengineloader.h"
#include <KConfigGroup>
#include <KSharedConfig>
#include <QComboBox>
#include <QHBoxLayout>
#include <QToolButton>

using namespace PimCommonTextTranslator;
TranslatorConfigureComboWidget::TranslatorConfigureComboWidget(QWidget *parent)
    : QWidget{parent}
    , mEngineComboBox(new QComboBox(this))
    , mConfigureEngine(new QToolButton(parent))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(QMargins{});

    mEngineComboBox->setObjectName(QStringLiteral("mEngineComboBox"));
    mainLayout->addWidget(mEngineComboBox);

    mConfigureEngine->setObjectName(QStringLiteral("mConfigureEngine"));
    mainLayout->addWidget(mConfigureEngine);
    mConfigureEngine->setEnabled(false); // Disable by default
    mConfigureEngine->setIcon(QIcon::fromTheme(QStringLiteral("settings-configure")));
    connect(mConfigureEngine, &QToolButton::clicked, this, &TranslatorConfigureComboWidget::slotConfigureEngine);
    connect(mEngineComboBox, &QComboBox::currentIndexChanged, this, &TranslatorConfigureComboWidget::slotEngineChanged);

    fillEngine();
}

TranslatorConfigureComboWidget::~TranslatorConfigureComboWidget() = default;

void TranslatorConfigureComboWidget::slotConfigureEngine()
{
    const QString engine = mEngineComboBox->currentData().toString();
    if (PimCommonTextTranslator::TranslatorEngineLoader::self()->hasConfigurationDialog(engine)) {
        PimCommonTextTranslator::TranslatorEngineLoader::self()->showConfigureDialog(engine, this);
    }
}

void TranslatorConfigureComboWidget::fillEngine()
{
    const QMap<QString, QString> map = PimCommonTextTranslator::TranslatorEngineLoader::self()->translatorEngineInfos();
    QMapIterator<QString, QString> iMap(map);
    while (iMap.hasNext()) {
        iMap.next();
        mEngineComboBox->addItem(iMap.value(), iMap.key());
    }
}

void TranslatorConfigureComboWidget::slotEngineChanged(int index)
{
    const QString engine = mEngineComboBox->itemData(index).toString();
    mConfigureEngine->setEnabled(PimCommonTextTranslator::TranslatorEngineLoader::self()->hasConfigurationDialog(engine));
    Q_EMIT engineChanged(engine);
}

void TranslatorConfigureComboWidget::load()
{
    KConfigGroup groupTranslate(KSharedConfig::openConfig(), TranslatorUtil::groupTranslateName());
    const QString engine = groupTranslate.readEntry(TranslatorUtil::engineTranslateName(), TranslatorUtil::defaultEngineName()); // Google by default
    const int index = mEngineComboBox->findData(engine);
    if (index != -1) {
        mEngineComboBox->setCurrentIndex(index);
    }
}

void TranslatorConfigureComboWidget::save()
{
    const QString engine = mEngineComboBox->currentData().toString();
    KConfigGroup groupTranslate(KSharedConfig::openConfig(), TranslatorUtil::groupTranslateName());
    groupTranslate.writeEntry(TranslatorUtil::engineTranslateName(), engine);
}
