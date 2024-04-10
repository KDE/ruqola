/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureactivitieswidget.h"
#include <KLocalizedString>
#include <PlasmaActivities/ActivitiesModel>
#include <QCheckBox>
#include <QItemDelegate>
#include <QLabel>
#include <QListView>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
namespace
{
// Code stolen from plasma-vault
class CheckboxDelegate : public QItemDelegate
{
public:
    CheckboxDelegate(QObject *parent)
        : QItemDelegate(parent)
    {
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        auto wholeRect = option.rect;

        // Drawing the checkbox
        auto checkRect = wholeRect;
        checkRect.setWidth(checkRect.height());
        drawCheck(painter, option, checkRect, option.state & QStyle::State_Selected ? Qt::Checked : Qt::Unchecked);

        // Drawing the text
        auto textRect = wholeRect;
        textRect.setLeft(textRect.left() + 8 + textRect.height());
        drawDisplay(painter, option, textRect, index.data(Qt::DisplayRole).toString());
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        Q_UNUSED(option);
        Q_UNUSED(index);
        return QSize(100, 22);
    }
};
}

ConfigureActivitiesWidget::ConfigureActivitiesWidget(QWidget *parent)
    : QWidget{parent}
    , mListView(new QListView(this))
    , mEnableActivitiesSupport(new QCheckBox(i18n("Limit to the selected activities:"), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18n("If you limit this account to activity, it will be shown in Ruqola only when you are in those activities. Furthermore, when "
                                 "you switch to an activity it should not be available in, it will automatically be hidden."),
                            this);
    label->setWordWrap(true);
    label->setObjectName("label"_L1);
    mainLayout->addWidget(label);

    mEnableActivitiesSupport->setObjectName("mEnableActivitiesSupport"_L1);
    mainLayout->addWidget(mEnableActivitiesSupport);
    mEnableActivitiesSupport->setChecked(false);

    mListView->setObjectName("mListView"_L1);
    mListView->setModel(new KActivities::ActivitiesModel(this));
    mListView->setItemDelegate(new CheckboxDelegate(this));
    mainLayout->addWidget(mListView);
    mListView->setSelectionMode(QAbstractItemView::MultiSelection);
    mListView->setEnabled(false);
    connect(mEnableActivitiesSupport, &QCheckBox::clicked, mListView, &QListView::setEnabled);
}

ConfigureActivitiesWidget::~ConfigureActivitiesWidget() = default;

QStringList ConfigureActivitiesWidget::activities() const
{
    const auto selection = mListView->selectionModel();
    const auto selected = selection->selectedIndexes();
    QStringList selectedActivities;
    selectedActivities.reserve(selected.count());
    for (const auto &selectedIndex : selected) {
        selectedActivities << selectedIndex.data(KActivities::ActivitiesModel::ActivityId).toString();
    }
    return selectedActivities;
}

void ConfigureActivitiesWidget::setActivities(const QStringList &lst)
{
    auto model = mListView->model();
    auto selection = mListView->selectionModel();
    selection->clearSelection();

    bool listIsEmpty{lst.isEmpty()};
    mListView->setEnabled(!listIsEmpty);
    if (!listIsEmpty) {
        for (int row = 0; row < model->rowCount(); ++row) {
            const auto index = model->index(row, 0);
            const auto activity = model->data(index, KActivities::ActivitiesModel::ActivityId).toString();

            if (lst.contains(activity)) {
                selection->select(index, QItemSelectionModel::Select);
            }
        }
    }
}

#include "moc_configureactivitieswidget.cpp"
