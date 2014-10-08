#include "widget.h"
#include <QApplication>
#include <QFile>
#include <QDateTime>
#include <QMessageBox>
#include <QPushButton>

bool firstStart = true;
bool showMessage = true;

void debugMessage(QtMsgType type, const char *cmsg) {
	QString msg = QString::fromUtf8(cmsg);
	qInstallMsgHandler(0);
	QString messageType;
	QMessageBox::Icon icon = QMessageBox::NoIcon;

	switch (type) {
		case QtDebugMsg:
			qDebug() << msg;
			messageType = "Info";
			icon = QMessageBox::Information;
			break;
		case QtWarningMsg:
			messageType = "Warning";
			icon = QMessageBox::Warning;
			break;
		case QtCriticalMsg:
			messageType = "Error";
			icon = QMessageBox::Critical;
			break;
		case QtFatalMsg:
			messageType = "Epic error";
			icon = QMessageBox::Critical;
	}

	QFile file("err.log");
	if (file.open(QIODevice::Append | QIODevice::Text)) {
		QTextStream out(&file);
		if (firstStart) {
			if (out.pos())
				out << "-----------------------------------------\n\n";
			firstStart = false;
		}
		out << "[ "+messageType + " | "+
			   QDateTime::currentDateTime().toString("dd.MM.yy h:mm:ss") +
			   " ]   " + msg + "\n\n";
		file.close();
	}

	if (showMessage && icon==QMessageBox::Warning) {
		QMessageBox m;
		m.setIcon(icon);
		m.setWindowTitle(messageType);
		QPushButton ok("Continue");
		QPushButton cancel("Don't show again");
		m.setText(msg);
		m.setInformativeText("You can see all messages in err.log file");
		m.addButton(&ok, QMessageBox::RejectRole);
		m.addButton(&cancel, QMessageBox::AcceptRole);
		m.setDefaultButton(&ok);
		if (m.exec())
			showMessage = false;
	}

	if (type==QtCriticalMsg || type==QtFatalMsg) {
		QMessageBox m;
		m.setIcon(icon);
		m.setWindowTitle(messageType);
		QPushButton ok("Exit");
		m.setText(msg);
		m.setInformativeText("You can see all messages in err.log file");
		m.addButton(&ok, QMessageBox::RejectRole);
		m.exec();
	}

	if (type==QtCriticalMsg)
		exit(EXIT_FAILURE);
	if (type==QtFatalMsg)
		abort();

	qInstallMsgHandler(debugMessage);
}


int main(int argc, char *argv[]) {
	// So easier for me ^__^
	static_assert(sizeof(GLuint)==sizeof(unsigned int),
				  "GLuint must be equal unsigned int");
	static_assert(sizeof(GLint)==sizeof(int), "GLint must be equal int");
	static_assert(sizeof(GLushort)==sizeof(unsigned short),
				  "GLushort must be equal unsigned short");
	static_assert(sizeof(GLshort)==sizeof(short), "GLshort must be equal short");
	static_assert(sizeof(GLfloat)==sizeof(float), "GLfloat must be equal float");
	static_assert(sizeof(QVector3D)==sizeof(float)*3,
				  "QVector3D must be equal float*3");
	static_assert(sizeof(QVector2D)==sizeof(float)*2,
				  "QVector2D must be equal float*2");

	QApplication a(argc, argv);
	qInstallMsgHandler(debugMessage);

	GLWidget w;
	w.setMinimumSize(800, 600);
	w.resize(1280, 720);
	w.show();

	return a.exec();
}
