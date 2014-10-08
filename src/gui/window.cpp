#include "window.h"
#include "../debug.h"

Window::Window(QGLWidget *glWidget): glWidget(glWidget) {
	plate.setBorderSize(40);
	plate.setTheme(0);
}

Window::~Window() {
	delete root;
}

void Window::setPosition(const QPoint &position) {
	_position = position;
	update();
}

void Window::setGeometry(const QRectF &geometry, float borderSize) {
	plate.setGeometry(geometry, borderSize);
	update();
}

void Window::setSize(const QSizeF &size) {
	plate.setSize(size);
	update();
}

void Window::setLayout(Layout *layout) {
	if (layout==nullptr) {
		qWarning() << "layout is null" << __DEBUG_INFO__;
		return;
	}

	delete root;
	root = layout;
	root->findElements(&items);

	update();
}

bool Window::contains(const QPoint &point) {
	return plate.geometry().contains(point-_position);
}

void Window::mouseChanged(const QPointF &pos, MouseEvents::MouseEvent event) {
	QPointF localPos = pos-_position;
	for (GuiItem *item: items)
		item->mouseChanged(item->mapFromRoot(localPos), event);

	update();
}

void Window::setMaterial(GuiMaterial *material) {
	this->material = material;
}

void Window::update() {
	if (!root)
		return;

	root->updateGeometry();
	QRectF g = plate.geometry();
	g.setSize(root->size());
	plate.setGeometry(g);

	_needRender = true;
}

void Window::render() {
	if (!material) {
		qWarning() << "GUI: material is unassigned" << __DEBUG_INFO__;
		return;
	}

	_needRender = false;

	material->setThemeNum(plate.theme());
	material->bind();
	plate.setMaterial(material);
	plate.render();

	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE);
	for (GuiItem *item: items)
		item->render(material);
}

void Window::renderText() {
	for (GuiItem *item: items)
		item->renderText(material, glWidget, {0, glWidget->height()-size().height()});
}
