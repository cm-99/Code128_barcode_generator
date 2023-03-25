#ifndef CHARACTERVARIABLEBARCODEPARTGUI_H
#define CHARACTERVARIABLEBARCODEPARTGUI_H

#include <QFrame>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QSpinBox>

/**
 * GUI for adding character variable part of the barcode, including setting its generation parameters.
 */
class CharacterVariableBarcodePartGUI : public QFrame
{
    Q_OBJECT
public:
    explicit CharacterVariableBarcodePartGUI(QWidget *parent = nullptr);
    /**
     * Used to connect line edit sinnals to slots managing barcode preview updates.
     * @return - returns pointer to line edit used to add fixed barcode part.
     */
    QLineEdit *get_line_edit();
    int get_increment_value();
    int get_steps_value();

private:
    QHBoxLayout layout;

    QLabel first_character_label;
    QLineEdit first_character_edit;

    QLabel increment_value_label;
    QSpinBox increment_value_spinbox;

    QLabel steps_value_label;
    QSpinBox steps_value_spinbox;

    QLabel last_character_label;

    QToolButton remove_button;

signals:
    /**
     * Used to connect line edit singnals to slots managing barcode preview updates.
     * @return - returns pointer to line edit used to add fixed barcode part.
     */
    void signal_remove_barcode_part();

private slots:
    /**
     * Updates last number (last_number_label) depending on the changes to first_number_edit, increment_value_spinbox
     * and steps_value_spinbox.
     * @param garbage_string - not used - connecting signals textChanged to this slot requires QString argument
     */
    void slot_update_last_character(QString garbage_string);
};

#endif // CHARACTERVARIABLEBARCODEPARTGUI_H
