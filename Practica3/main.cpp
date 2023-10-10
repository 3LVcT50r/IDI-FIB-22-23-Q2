#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include "MyForm.h"
int main( int argc, char ** argv )
{
    QApplication app( argc, argv );
    MyForm form;
    form.show();
    return app.exec();
}