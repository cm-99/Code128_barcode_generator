#ifndef BARCODESETTINGSGUI_H
#define BARCODESETTINGSGUI_H

#include "barcodesettingsmanager.h"

#include <QFrame>
#include <QObject>
#include <QFormLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>

/**
 * GUI for changing barcode settings.
 * Its changes should influence barcode preview if properly connected.
 */
class BarcodeSettingsGUI : public QFrame
{
    Q_OBJECT
public:
    explicit BarcodeSettingsGUI(QWidget *parent = nullptr);
    ~BarcodeSettingsGUI();
    /**
     * Returns BarcodeSettings struct holding current settings modified through this class.
     */
    BarcodeSettings get_current_barcode_settings();

private:
    BarcodeSettings barcode_settings;

    //GUI attributes
    QFormLayout settings_layout;

    QComboBox barcode_type_combobox;

    QCheckBox text_visibility_checkbox;
    QCheckBox text_division_checkbox;

    QSpinBox barcode_size_spinbox;
    QSpinBox barcode_height_in_mm_spinbox;
    QSpinBox text_font_size_spinbox;
    QSpinBox text_part_size_spinbox;

    QList<QCheckBox*> barcode_settings_checkboxes;
    QList<QSpinBox*> barcode_settings_spinboxes;

    /**
     * Creates input widgets for barcode_settings adjustment, sets their initial values to those from current settings.
     * Connects all inputs to slot_update_settings.
     */
    void create_input_widgets();

private slots:
    /**
     * Updates barcode_settings in response to settings modification through input widgets.
     */
    void slot_update_settings(int new_value);

signals:
    /**
     * To be connected to corresponding slot in mainwindow updating barcode's preview.
     */
    void signal_settings_updated(BarcodeSettings new_barcode_settings);
};

#endif // BARCODESETTINGSGUI_H
