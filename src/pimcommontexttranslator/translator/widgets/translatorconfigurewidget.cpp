/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translatorconfigurewidget.h"
#include "translator/misc/translatorutil.h"
#include "translator/translatorengineloader.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QComboBox>
#include <QLabel>
#include <QStackedWidget>
#include <QVBoxLayout>
using namespace PimCommonTextTranslator;

class Q_DECL_HIDDEN PimCommonTextTranslator::TranslatorConfigureWidget::TranslatorConfigureWidgetPrivate
{
public:
    TranslatorConfigureWidgetPrivate(TranslatorConfigureWidget *parent)
        : mEngineComboBox(new QComboBox(parent))
        , mStackedWidget(new QStackedWidget(parent))
        , mEmptyWidget(new QWidget(parent))
        , mLibreTranslateWidget(new QWidget(parent))
    {
    }

    QComboBox *const mEngineComboBox;
    QStackedWidget *const mStackedWidget;
    QWidget *const mEmptyWidget;
    QWidget *const mLibreTranslateWidget;
};

TranslatorConfigureWidget::TranslatorConfigureWidget(QWidget *parent)
    : QWidget{parent}
    , d(new TranslatorConfigureWidgetPrivate(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    d->mEngineComboBox->setObjectName(QStringLiteral("mEngineComboBox"));
    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    auto label = new QLabel(i18n("Engine:"), this);
    label->setObjectName(QStringLiteral("label"));
    hboxLayout->addWidget(label);
    hboxLayout->addWidget(d->mEngineComboBox);

    d->mStackedWidget->setObjectName(QStringLiteral("mStackedWidget"));
    mainLayout->addWidget(d->mStackedWidget);

    d->mEmptyWidget->setObjectName(QStringLiteral("mEmptyWidget"));
    d->mStackedWidget->addWidget(d->mEmptyWidget);

    d->mLibreTranslateWidget->setObjectName(QStringLiteral("mLibreTranslateWidget"));
    d->mStackedWidget->addWidget(d->mLibreTranslateWidget);

    connect(d->mEngineComboBox, &QComboBox::currentIndexChanged, this, &TranslatorConfigureWidget::switchEngine);
    d->mStackedWidget->setCurrentWidget(d->mEmptyWidget);
    fillEngine();
}

TranslatorConfigureWidget::~TranslatorConfigureWidget() = default;

void TranslatorConfigureWidget::fillEngine()
{
    const QMap<QString, QString> map = PimCommonTextTranslator::TranslatorEngineLoader::self()->translatorEngineInfos();
    QMapIterator<QString, QString> iMap(map);
    while (iMap.hasNext()) {
        iMap.next();
        d->mEngineComboBox->addItem(iMap.value(), iMap.key());
    }
}

void TranslatorConfigureWidget::saveSettings()
{
    TranslatorUtil::saveEngineSettings(d->mEngineComboBox->currentData().toString());
}

void TranslatorConfigureWidget::loadSettings()
{
    const QString engine = TranslatorUtil::loadEngine();
    const int index = d->mEngineComboBox->findData(engine);
    if (index != -1) {
        d->mEngineComboBox->setCurrentIndex(index);
    }
}

void TranslatorConfigureWidget::switchEngine(int index)
{
    // TODO
}
