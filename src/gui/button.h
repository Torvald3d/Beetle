#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include <QTimer>
#include <QString>
#include "guiitem.h"
#include "label.h"

class Button: public QObject, public GuiItem {
Q_OBJECT
	QTimer timer;
	Label *label = nullptr;
	bool _pressed = false;
	float borderSize = 15;
	bool repeat = false;
	const int themePress = 2;
	const int themeRelease = 1;
	int firstDelay = 400;
	int repeatDelay = 80;

private slots:
	void repeatClick();

signals:
	void press();
	void release();

public:
	Button(const QString &text="", const QSizeF &size={120, 50});
	virtual ~Button();

	void setText(const QString &text);
	void enableRepeat(bool repeat);
	void setRepeatDelay(int repeatDelay);

	virtual void updateGeometry() override;
	virtual void mouseChanged(const QPointF &pos, MouseEvent event) override;
	virtual void render(GuiMaterial *material) override;
	virtual void renderText(GuiMaterial *material, QGLWidget *glWidget,
							const QPointF &windowShift) override;

	inline const QString &text() const {
		return label->text();
	}

	inline bool pressed() const {
		return _pressed;
	}

};

#endif // BUTTON_H
