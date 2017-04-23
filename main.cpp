#include <QtWidgets>
#include "AudioPlayer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    AudioPlayer AudioPlayer;
    QFile file(":/style/style.qss");
    file.open(QFile::ReadOnly);
    QString strCSS = QLatin1String(file.readAll());
    qApp->setStyleSheet(strCSS);


    AudioPlayer.resize(640, 200);
    AudioPlayer.show();

    return app.exec();
}