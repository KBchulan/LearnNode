#ifndef VIEW_WIDGET_H
#define VIEW_WIDGET_H

#include <QWidget>

class ViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ViewWidget(QWidget *parent = nullptr);
    virtual ~ViewWidget() = default;
};

#endif // VIEW_WIDGET_H