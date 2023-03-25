#ifndef BARCODEENCODER_H
#define BARCODEENCODER_H

#include <QList>

/**
 * Interface for barcode encoders.
 * For implementation guidelines and methods description see Code128Encoder.
*/
class BarcodeEncoder
{
public:
    virtual ~BarcodeEncoder(){};
    virtual QList<int> encode(const QString data) = 0;
};

#endif // BARCODEENCODER_H
