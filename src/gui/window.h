#ifndef WINDOW_H
#define WINDOW_H

#include "guiitem.h"
#include "button.h"

class Window: public MouseEvents {
	QGLWidget *glWidget;
	QList<GuiItem *> items;
	Layout *root = nullptr;
	Plate plate;
	QPoint _position;
	bool _needRender = false;
	GuiMaterial *material = nullptr;

public:
	Window(QGLWidget *glWidget);
	virtual ~Window();

	void setPosition(const QPoint &position);
	void setGeometry(const QRectF &geometry, float borderSize = -1);
	void setSize(const QSizeF &size);
	void setLayout(Layout *layout);
	bool contains(const QPoint &point);
	virtual void mouseChanged(const QPointF &pos, MouseEvent event) override;

	inline QSizeF size() const {
		return plate.size();
	}

	inline const QPoint &position() const {
		return _position;
	}

	inline bool needRender() const {
		return _needRender;
	}

	void setMaterial(GuiMaterial *material);
	void update();
	void render();
	void renderText();

};

#endif // WINDOW_H
