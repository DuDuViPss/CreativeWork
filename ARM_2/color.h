#ifndef COLOR_H
#define COLOR_H
#include <QStyledItemDelegate>
#include <QPainter>
#include <QModelIndex>
#include <QApplication>

class MultiColorDelegate : public QStyledItemDelegate
{
public:
    MultiColorDelegate(QObject *parent = nullptr)
        : QStyledItemDelegate(parent)
    {
    }

    void setRowColor(int row, const QColor &color)
    {
        rowColors[row] = color;
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        if (rowColors.contains(index.row())) {
            painter->save();

            // Закрашиваем цветом сначала
            painter->fillRect(option.rect, rowColors[index.row()]);

            // Затем рисуем текст поверх цвета
            QStyleOptionViewItem opt = option;
            initStyleOption(&opt, index);

            // Рисуем текст
            opt.textElideMode = Qt::ElideNone; // Чтобы текст не обрезался
            QRect textRect = opt.rect.adjusted(2, 0, 0, 0); // Отступы для текста
            QApplication::style()->drawItemText(painter, textRect, opt.displayAlignment, opt.palette, opt.state & QStyle::State_Enabled, index.data().toString());

            painter->restore();
        } else {
            // Если цвет не установлен, просто рисуем стандартно
            QStyledItemDelegate::paint(painter, option, index);
        }
    }

private:
    QMap<int, QColor> rowColors;
};
#endif // COLOR_H
