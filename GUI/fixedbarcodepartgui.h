#ifndef FIXEDBARCODEPARTGUI_H
#define FIXEDBARCODEPARTGUI_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>

//An interface could be extracted from FixedBarcodePartGUI/*VariableBarcodePartGUI classes to remove redundant comments.
//For now it seems like unnecessary code coupling.

/**
 * GUI for adding fixed/constant part of the barcode.
 */
class FixedBarcodePartGUI : public QFrame
{
    Q_OBJECT
public:
    explicit FixedBarcodePartGUI(QWidget *parent = nullptr);
    /**
     * Used to connect line edit signals to slots managing barcode preview updates.
     * @return - returns pointer to line edit used to add fixed barcode part.
     */
    QLineEdit *get_line_edit();

private:
    QHBoxLayout layout;
    QLabel barcode_part_label;
    QLineEdit barcode_part_edit;
    QToolButton remove_button;

signals:
    /**
     * To be connected to mainwindow's slot removing object emitting this signal.
     */
    void signal_remove_barcode_part();
};

#endif // FIXEDBARCODEPARTGUI_H
