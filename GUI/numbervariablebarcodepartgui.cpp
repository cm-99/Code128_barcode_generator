#include "GUI/numbervariablebarcodepartgui.h"

NumberVariableBarcodePartGUI::NumberVariableBarcodePartGUI(QWidget *parent) :
    QFrame(parent)
{
    setLayout(&layout);
    setStyleSheet("QToolButton {font-weight: bold;}");
    setFrameStyle(QFrame::Panel | QFrame::Raised);

    first_number_label.setText("First number");
    increment_value_label.setText("Increment value");
    steps_value_label.setText("Steps");
    last_number_label.setText("Last number: ");
    last_number_label.setStyleSheet("font-weight: bold;");
    remove_button.setText("X");

    layout.addWidget(&first_number_label);
    layout.addWidget(&first_number_edit);
    layout.addWidget(&increment_value_label);
    layout.addWidget(&increment_value_spinbox);
    layout.addWidget(&steps_value_label);
    layout.addWidget(&steps_value_spinbox);
    layout.addWidget(&last_number_label);
    layout.addWidget(&remove_button);

    first_number_edit.setParent(this);
    first_number_edit.setValidator(new QIntValidator(0, 100, this));

    connect(&remove_button, &QToolButton::clicked, this, &NumberVariableBarcodePartGUI::signal_remove_barcode_part);

    connect(&first_number_edit, &QLineEdit::textChanged, this, &NumberVariableBarcodePartGUI::slot_update_last_number);
    connect(&increment_value_spinbox, &QSpinBox::textChanged, this, &NumberVariableBarcodePartGUI::slot_update_last_number);
    connect(&steps_value_spinbox, &QSpinBox::textChanged, this, &NumberVariableBarcodePartGUI::slot_update_last_number);

}

QLineEdit *NumberVariableBarcodePartGUI::get_line_edit()
{
    return &first_number_edit;
}

int NumberVariableBarcodePartGUI::get_increment_value()
{
    return increment_value_spinbox.value();
}

int NumberVariableBarcodePartGUI::get_steps_value()
{
    return steps_value_spinbox.value();
}

void NumberVariableBarcodePartGUI::slot_update_last_number([[maybe_unused]] QString garbage_string)
{
    int first_number = first_number_edit.text().toInt();

    int increment_value = increment_value_spinbox.value();
    int steps_value = steps_value_spinbox.value();

    int last_number = first_number + steps_value * increment_value;

    QString last_number_label_text = "Last number: " + QString::number(last_number);
    last_number_label.setText(last_number_label_text);
}
