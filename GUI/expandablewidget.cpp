#include <QPropertyAnimation>

#include "GUI/expandablewidget.h"

ExpandableWidget::ExpandableWidget(const QString &title, const int animation_duration, QWidget *parent) :
    QWidget{parent},
    main_layout(new QGridLayout(this)),
    toggle_button(new QToolButton(this)),
    header_line(new QFrame(this)),
    toggle_animation(new QParallelAnimationGroup(this)),
    content_area(new QScrollArea(this)),
    animation_duration(animation_duration)
{
    toggle_button->setStyleSheet("QToolButton { border: none; }");
    toggle_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toggle_button->setArrowType(Qt::ArrowType::RightArrow);
    toggle_button->setText(title);
    toggle_button->setCheckable(true);
    toggle_button->setChecked(false);

    header_line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    content_area->setStyleSheet("QScrollArea { border: none; }");
    content_area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //Start out collapsed
    content_area->setMaximumHeight(0);
    content_area->setMinimumHeight(0);

    //Let the entire widget grow and shrink with its content
    toggle_animation->addAnimation(new QPropertyAnimation(this, "minimumHeight"));
    toggle_animation->addAnimation(new QPropertyAnimation(this, "maximumHeight"));
    toggle_animation->addAnimation(new QPropertyAnimation(content_area, "maximumHeight"));

    main_layout->setVerticalSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);

    int row = 0;
    main_layout->addWidget(toggle_button, row, 0, 1, 1, Qt::AlignLeft);
    main_layout->addWidget(header_line, row++, 2, 1, 1);
    main_layout->addWidget(content_area, row, 0, 1, 3);
    setLayout(main_layout);

    connect(toggle_button, &QToolButton::toggled, this, &ExpandableWidget::slot_toggle);
}

void ExpandableWidget::set_content_layout(QLayout &content_layout)
{
    delete content_area->layout();
    content_area->setLayout(&content_layout);
    collapsed_height = sizeHint().height() - content_area->maximumHeight();

    update_heights();
}

void ExpandableWidget::set_title(QString title)
{
    toggle_button -> setText(title);
}

void ExpandableWidget::update_heights()
{
    int content_height = content_area->layout()->sizeHint().height();

    for (int i = 0; i < toggle_animation->animationCount() - 1; ++i)
    {
        QPropertyAnimation* section_animation = static_cast<QPropertyAnimation *>(toggle_animation->animationAt(i));
        section_animation ->setDuration(animation_duration);
        section_animation ->setStartValue(collapsed_height);
        section_animation ->setEndValue(collapsed_height + content_height);
    }

    QPropertyAnimation* content_animation =
            static_cast<QPropertyAnimation *>(toggle_animation->animationAt(toggle_animation->animationCount() - 1));
    content_animation->setDuration(animation_duration);
    content_animation->setStartValue(0);
    content_animation->setEndValue(content_height);

    toggle_animation->setDirection(is_expanded ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    toggle_animation->start();
}

void ExpandableWidget::slot_toggle(bool checked)
{
    toggle_button->setArrowType(checked ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);
    toggle_animation->setDirection(checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    toggle_animation->start();

    this->is_expanded = checked;
}
