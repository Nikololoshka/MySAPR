#include <QtWidgets>

#include "mySAPRWindow.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/icon/app.png"));

    MySAPRWindow window;
    window.show();

    return app.exec();
}
