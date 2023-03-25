#ifndef EXPANDABLEWIDGET_H
#define EXPANDABLEWIDGET_H

#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>

/**
 * Widget that can expand/collapse on title press.
 */
class ExpandableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExpandableWidget(const QString &title = "", const int animation_duration = 300, QWidget *parent = 0);
    /**
     * Sets layout to be used as expandable section.
     */
    void set_content_layout(QLayout &content_layout);
    /**
     * Updates animations and their heights
     */
    void update_heights();
    void set_title(QString title);

private:
    QGridLayout *main_layout;
    QToolButton *toggle_button;
    QFrame *header_line;
    QParallelAnimationGroup *toggle_animation;
    QScrollArea *content_area;
    int animation_duration;
    int collapsed_height;
    bool is_expanded = false;

private slots:
    /**
     * Toggles the expandable widget on toggle_button click.
     */
    void slot_toggle(bool checked);
};

#endif // EXPANDABLEWIDGET_H
