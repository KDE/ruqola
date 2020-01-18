#ifndef MESSAGELISTDELEGATE_H
#define MESSAGELISTDELEGATE_H

#include <QItemDelegate>

class MessageListDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit MessageListDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // MESSAGELISTDELEGATE_H
