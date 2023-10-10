#ifndef MYLCDNUMBER_H
#define MYLCDNUMBER_H

#include <QLCDNumber>

class MyLCDNumber : public QLCDNumber {
    Q_OBJECT

    public:
        LCDNumber(QWidget *parent);
        int count = 0;

    public slots:
        void setValue();
        void clearValue();
    
    signals:
};

#endif