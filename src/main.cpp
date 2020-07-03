// main.cpp
#include <QHBoxLayout>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuick/QQuickWindow>
#include <QtWidgets/QApplication>
#include <QtWidgets>

#include "mainwindow.h"
int
main(int argc, char* argv[])
{
  qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

  if (qEnvironmentVariableIsEmpty("QTGLESSTREAM_DISPLAY")) {
    qputenv("QT_QPA_EGLFS_PHYSICAL_WIDTH", QByteArray("213"));
    qputenv("QT_QPA_EGLFS_PHYSICAL_HEIGHT", QByteArray("120"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  }
  QApplication app(argc, argv);
  app.setApplicationName("Qml Responsive Livereload");
  app.setApplicationVersion("1.0.0");

  QFont font("Roboto");
  font.setPixelSize(20);
  app.setFont(font);

  MainWindow w;
  w.show();

  return app.exec();
}
