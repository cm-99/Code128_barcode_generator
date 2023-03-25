#include "GUI/fixedbarcodepartgui.h"

FixedBarcodePartGUI::FixedBarcodePartGUI(QWidget *parent)
    : QFrame{parent}
{
    setLayout(&layout);
    setStyleSheet("QToolButton {font-weight: bold;}");
    setFrameStyle(QFrame::Panel | QFrame::Raised);

    barcode_part_label.setText("Fixed barcode part");
    remove_button.setText("X");

    layout.addWidget(&barcode_part_label);
    layout.addWidget(&barcode_part_edit);
    layout.addWidget(&remove_button);

    //Used to get index of widget in layout barcode_parts_layout which is holding QLineEdit that send signal textEdited
    barcode_part_edit.setParent(this);
    connect(&remove_button, &QToolButton::clicked, this, &FixedBarcodePartGUI::signal_remove_barcode_part);
}

QLineEdit *FixedBarcodePartGUI::get_line_edit()
{
    return &barcode_part_edit;
}
