#ifndef GUIITEM_H
#define GUIITEM_H

#include "materials/guimaterial.h"
#include "layout.h"
#include "plate.h"
#include <QGLWidget>

class MouseEvents {
public:
	enum MouseEvent {Press, Move, Release};
	virtual void mouseChanged(const QPointF &pos, MouseEvent event) = 0;
};


class GuiItem: public Layout, public MouseEvents {
protected:
	Plate _plate;

public:
	GuiItem();
	virtual ~GuiItem();

	inline const Plate &plate() const {
		return _plate;
	}

	virtual void render(GuiMaterial *material) = 0;
	virtual void renderText(GuiMaterial *material, QGLWidget *glWidget,
							const QPointF &windowShift) = 0;
};

#endif // GUIITEM_H
