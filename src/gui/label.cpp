#include "label.h"

Label::Label(Align align, const QString &text, const QSizeF &size) {
	setAlign(align);
	_geometry.setSize(size);
	font.setPixelSize(16);
	font.setFamily("Arial");
	font.setBold(true);
	fontMetric = new QFontMetrics(font);
	setText(text);
}

Label::~Label() {
	delete fontMetric;
}

void Label::updateGeometry() {

}

void Label::mouseChanged(const QPointF &, MouseEvents::MouseEvent) {

}

void Label::setText(const QString &text) {
	_text = text;
}

void Label::setAlign(Label::Align align) {
	this->align = align;
}

void Label::render(GuiMaterial *) {

}

void Label::renderText(GuiMaterial *material, QGLWidget *glWidget,
					   const QPointF &windowShift) {
	Material::unbind();
	QPointF textPos = mapToRoot(_geometry.topLeft()) + windowShift;

	QRect br = fontMetric->boundingRect(_text);
	if (align==Center)
		textPos.rx() += (_geometry.width()-br.width())/2;
	if (align==Right)
		textPos.rx() += _geometry.width()-br.width();
	textPos.ry() += (_geometry.height()+br.height()/2)/2+2;

	if (material->tone()>127)
		glColor4f(1.0, 1.0, 1.0, 0.35);
	else
		glColor4f(0.0, 0.0, 0.0, 0.35);
	glWidget->renderText(textPos.x()-1, textPos.y()+1, _text, font);

	if (material->tone()>127)
		glColor4f(0.0, 0.0, 0.0, 0.8);
	else
		glColor4f(1.0, 1.0, 1.0, 0.7);
	glWidget->renderText(textPos.x(), textPos.y(), _text, font);
}
