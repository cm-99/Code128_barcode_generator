#ifndef NUMBERVARIABLEBARCODEPARTGUI_H
#define NUMBERVARIABLEBARCODEPARTGUI_H

#include <QFrame>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QToolButton>

/**
 * GUI for adding number variable part of the barcode, including setting its generation parameters.
 */
class NumberVariableBarcodePartGUI : public QFrame
{
    Q_OBJECT
public:
    explicit NumberVariableBarcodePartGUI(QWidget *parent = nullptr);
    /**
     * Used to connect line edit singnals to slots managing barcode preview updates.
     * @return - returns pointer to line edit used to add fixed barcode part.
     */
    QLineEdit *get_line_edit();
    int get_increment_value();
    int get_steps_value();

private:
    QHBoxLayout layout;

    QLabel first_number_label;
    QLineEdit first_number_edit;

    QLabel increment_value_label;
    QSpinBox increment_value_spinbox;

    QLabel steps_value_label;
    QSpinBox steps_value_spinbox;

    QLabel last_number_label;

    QToolButton remove_button;

signals:
    /**
     * To be connected to mainwindow's slot removing object emitting this signal.
     */
    void signal_remove_barcode_part();

private slots:
    /**
     * Updates last number (last_number_label) depending on the changes to first_number_edit, increment_value_spinbox
     * and steps_value_spinbox.
     * @param garbage_string - not used - connecting signals textChanged to this slot requires QString argument
     */
    void slot_update_last_number(QString garbage_string);
};

#endif // NUMBERVARIABLEBARCODEPARTGUI_H
