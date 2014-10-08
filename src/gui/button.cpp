#include "button.h"
#include "../debug.h"

Button::Button(const QString &text, const QSizeF &size) {
	_plate.setTheme(themeRelease);
	_geometry.setSize(size);
	label = new Label(Label::Center, text);
	label->setText(text);
	addItem(label);
	updateGeometry();
	connect(&timer, SIGNAL(timeout()), this, SLOT(repeatClick()));
}

Button::~Button() {

}

void Button::setText(const QString &text) {
	label->setText(text);
}

void Button::enableRepeat(bool repeat) {
	this->repeat = repeat;
}

void Button::setRepeatDelay(int repeatDelay) {
	this->repeatDelay = repeatDelay;
}

void Button::updateGeometry() {
	_plate.setGeometry(_geometry, mapToRoot(QPointF(0, 0)), borderSize);
	label->setSize(_geometry.size());
}

void Button::mouseChanged(const QPointF &pos, MouseEvents::MouseEvent event) {
	if (_geometry.contains(pos)) {
		if (event==Press) {
			_pressed = true;
			_plate.setTheme(themePress);
			emit press();
			if (repeat)
				timer.start(firstDelay);
		}
		if (event==Move && _pressed)
			_plate.setTheme(themePress);

		if (event==Release && _pressed)
			emit release();
	}

	if (event==Release) {
		_pressed = false;
		_plate.setTheme(themeRelease);
		timer.stop();
	}
}

void Button::render(GuiMaterial *material) {
	material->setThemeNum(_plate.theme());
	material->bind();
	_plate.setMaterial(material);
	_plate.render();
}

void Button::renderText(GuiMaterial *material, QGLWidget *glWidget,
						const QPointF &windowShift) {
	label->renderText(material, glWidget, windowShift);
}


void Button::repeatClick() {
	release();
	timer.start(repeatDelay);
}
