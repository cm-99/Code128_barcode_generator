#include "GUI/barcodesettingsgui.h"

void BarcodeSettingsGUI::create_input_widgets()
{
    barcode_type_combobox.addItem("Code128", BarcodeType::Code128);
    barcode_type_combobox.setCurrentIndex(barcode_settings.barcode_type);
    settings_layout.addRow(tr("Barcode type:"), &barcode_type_combobox);
    connect(&barcode_type_combobox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BarcodeSettingsGUI::slot_update_settings);

    if(barcode_settings.text_is_visible)
        text_visibility_checkbox.setCheckState(Qt::CheckState::Checked);
    settings_layout.addRow(tr("&Barcode text visible:"), &text_visibility_checkbox);
    connect(&text_visibility_checkbox, &QCheckBox::stateChanged, this, &BarcodeSettingsGUI::slot_update_settings);

    if(barcode_settings.text_is_to_be_divided)
        text_division_checkbox.setCheckState(Qt::CheckState::Checked);
    settings_layout.addRow(tr("&Divide text into parts:"), &text_division_checkbox);
    connect(&text_division_checkbox, &QCheckBox::stateChanged, this, &BarcodeSettingsGUI::slot_update_settings);

    barcode_size_spinbox.setValue(barcode_settings.barcode_width_scale);
    barcode_size_spinbox.setRange(0, 10);
    settings_layout.addRow(tr("&Barcode width scale:"), &barcode_size_spinbox);
    connect(&barcode_size_spinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BarcodeSettingsGUI::slot_update_settings);

    barcode_height_in_mm_spinbox.setValue(barcode_settings.barcode_height_in_mm);
    barcode_height_in_mm_spinbox.setRange(0, 297);
    settings_layout.addRow(tr("&Barcode height [mm]:"), &barcode_height_in_mm_spinbox);
    connect(&barcode_height_in_mm_spinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BarcodeSettingsGUI::slot_update_settings);

    text_font_size_spinbox.setValue(barcode_settings.font_size);
    text_font_size_spinbox.setRange(0, 50);
    settings_layout.addRow(tr("&Font size:"), &text_font_size_spinbox);
    connect(&text_font_size_spinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BarcodeSettingsGUI::slot_update_settings);

    text_part_size_spinbox.setValue(barcode_settings.text_part_size);
    text_part_size_spinbox.setRange(1, 10);
    settings_layout.addRow(tr("&Text part size:"), &text_part_size_spinbox);
    connect(&text_part_size_spinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BarcodeSettingsGUI::slot_update_settings);
}

BarcodeSettingsGUI::BarcodeSettingsGUI(QWidget *parent) :
    QFrame{parent}
{
    barcode_settings = BarcodeSettingsManager::get_barcode_settings();
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLayout(&settings_layout);

    create_input_widgets();

    barcode_settings_checkboxes.append(&text_visibility_checkbox);
    barcode_settings_checkboxes.append(&text_division_checkbox);

    barcode_settings_spinboxes.append(&barcode_size_spinbox);
    barcode_settings_spinboxes.append(&barcode_height_in_mm_spinbox);
    barcode_settings_spinboxes.append(&text_font_size_spinbox);
    barcode_settings_spinboxes.append(&text_part_size_spinbox);
}

BarcodeSettingsGUI::~BarcodeSettingsGUI(){}

BarcodeSettings BarcodeSettingsGUI::get_current_barcode_settings()
{
    return barcode_settings;
}

void BarcodeSettingsGUI::slot_update_settings(int new_value)
{
    QObject *sender = QObject::sender();
    QCheckBox *possible_sender_checkbox = dynamic_cast<QCheckBox*>(sender);

    if(possible_sender_checkbox != nullptr)
    {
        if(barcode_settings_checkboxes.contains(possible_sender_checkbox))
        {
            int index = barcode_settings_checkboxes.indexOf(possible_sender_checkbox);
            if(index == 0)
            {
                barcode_settings.text_is_visible = possible_sender_checkbox -> isChecked();
            }
            else if(index == 1)
            {
                barcode_settings.text_is_to_be_divided = possible_sender_checkbox -> isChecked();
            }
        }

        emit signal_settings_updated(this -> barcode_settings);
        return;
    }

    QSpinBox *possible_sender_spinbox = dynamic_cast<QSpinBox*>(sender);
    if(barcode_settings_spinboxes.contains(possible_sender_spinbox))
    {
        int index = barcode_settings_spinboxes.indexOf(possible_sender_spinbox);
        if(index == 0)
            barcode_settings.barcode_width_scale = new_value;
        else if(index == 1)
            barcode_settings.barcode_height_in_mm = new_value;
        else if(index == 2)
            barcode_settings.font_size = new_value;
        else if(index == 3)
            barcode_settings.text_part_size = new_value;

        emit signal_settings_updated(this -> barcode_settings);
        return;
    }

    QComboBox *possible_sender_combobox = dynamic_cast<QComboBox*>(sender);
    if(possible_sender_combobox != nullptr)
    {
        barcode_settings.barcode_type = static_cast<BarcodeType>(new_value);
    }

    emit signal_settings_updated(this -> barcode_settings);
}
