#ifndef LABEL_H
#define LABEL_H

#include <QObject>
#include <QString>
#include "guiitem.h"

class Label: public QObject, public GuiItem {
	Q_OBJECT

public:
	enum Align {Left, Center, Right};

	Label(Align align, const QString &text="", const QSizeF &size={200, 50});
	virtual ~Label();

	virtual void updateGeometry() override;
	virtual void mouseChanged(const QPointF &pos, MouseEvent event) override;
	virtual void render(GuiMaterial *material) override;
	virtual void renderText(GuiMaterial *material, QGLWidget *glWidget,
							const QPointF &windowShift) override;
	void setText(const QString &text);
	void setAlign(Align align);

	inline const QString &text() const {
		return _text;
	}

private:
	QString _text;
	QFont font;
	Align align = Left;
	QFontMetrics *fontMetric = nullptr;
};

#endif // LABEL_H
