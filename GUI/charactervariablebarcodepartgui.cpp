#include "GUI/charactervariablebarcodepartgui.h"

CharacterVariableBarcodePartGUI::CharacterVariableBarcodePartGUI(QWidget *parent) :
    QFrame(parent)
{
    setLayout(&layout);
    setStyleSheet("QToolButton {font-weight: bold;}");
    setFrameStyle(QFrame::Panel | QFrame::Raised);

    first_character_label.setText("First character");
    increment_value_label.setText("Increment value");
    steps_value_label.setText("Steps");
    last_character_label.setText("Last character: ");
    last_character_label.setStyleSheet("font-weight: bold;");
    remove_button.setText("X");

    layout.addWidget(&first_character_label);
    layout.addWidget(&first_character_edit);
    layout.addWidget(&increment_value_label);
    layout.addWidget(&increment_value_spinbox);
    layout.addWidget(&steps_value_label);
    layout.addWidget(&steps_value_spinbox);
    layout.addWidget(&last_character_label);
    layout.addWidget(&remove_button);

    first_character_edit.setParent(this);
    first_character_edit.setMaxLength(1);

    connect(&remove_button, &QToolButton::clicked, this, &CharacterVariableBarcodePartGUI::signal_remove_barcode_part);

    connect(&first_character_edit, &QLineEdit::textChanged, this, &CharacterVariableBarcodePartGUI::slot_update_last_character);
    connect(&increment_value_spinbox, &QSpinBox::textChanged, this, &CharacterVariableBarcodePartGUI::slot_update_last_character);
    connect(&steps_value_spinbox, &QSpinBox::textChanged, this, &CharacterVariableBarcodePartGUI::slot_update_last_character);
}

QLineEdit *CharacterVariableBarcodePartGUI::get_line_edit()
{
    return &first_character_edit;
}

int CharacterVariableBarcodePartGUI::get_increment_value()
{
    return increment_value_spinbox.value();
}

int CharacterVariableBarcodePartGUI::get_steps_value()
{
    return steps_value_spinbox.value();
}

void CharacterVariableBarcodePartGUI::slot_update_last_character([[maybe_unused]] QString garbage_string)
{
    char first_character = first_character_edit.text().at(0).toLatin1();
    int first_character_ASCII_index = (int)first_character;

    int increment_value = increment_value_spinbox.value();
    int steps_value = steps_value_spinbox.value();

    int last_character_ASCII_index = first_character_ASCII_index + steps_value * increment_value;
    char last_character = (char)last_character_ASCII_index;

    QString last_character_label_text = "Last character: " + QString(QChar::fromLatin1(last_character));
    last_character_label.setText(last_character_label_text);
}
