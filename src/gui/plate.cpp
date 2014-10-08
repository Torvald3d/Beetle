#include "plate.h"
#include "../debug.h"

QVector <QVector2D> Plate::texCoord;
QVector <unsigned short> Plate::indices;
QGLBuffer Plate::tcBuff;
QGLBuffer Plate::indBuff;

Plate::Plate() {
	_geometry = QRectF(0, 0, 0, 0);
	vertices.fill({0, 0}, 16);

	if (vertBuff.create()) {
		vertBuff.setUsagePattern(QGLBuffer::DynamicDraw);
		vertBuff.bind();
		vertBuff.allocate(vertices.data(), vertices.count()*sizeof(QVector2D));
	}

	if (texCoord.count())
		return;

	// static init

	texCoord.reserve(16);

	texCoord.append({0, 1});
	texCoord.append({0, 0});
	texCoord.append({1, 0});
	texCoord.append({1, 1});

	texCoord.append({1, 1});
	texCoord.append({1, 0});
	texCoord.append({0, 0});
	texCoord.append({0, 1});

	texCoord.append({0, 0});
	texCoord.append({0, 1});
	texCoord.append({1, 1});
	texCoord.append({1, 0});

	texCoord.append({1, 0});
	texCoord.append({1, 1});
	texCoord.append({0, 1});
	texCoord.append({0, 0});


	indices.reserve(54);

	indices <<  0 <<  1 <<  2 <<  0 <<  2 <<  3;
	indices <<  3 <<  2 <<  5 <<  3 <<  5 <<  4;
	indices <<  4 <<  5 <<  6 <<  4 <<  6 <<  7;
	indices <<  1 <<  8 << 11 <<  1 << 11 <<  2;
	indices <<  2 << 11 << 12 <<  2 << 12 <<  5;
	indices <<  5 << 12 << 15 <<  5 << 15 <<  6;
	indices <<  8 <<  9 << 10 <<  8 << 10 << 11;
	indices << 11 << 10 << 13 << 11 << 13 << 12;
	indices << 12 << 13 << 14 << 12 << 14 << 15;

	tcBuff = QGLBuffer(QGLBuffer::VertexBuffer);
	indBuff = QGLBuffer(QGLBuffer::IndexBuffer);
	if (tcBuff.create() && indBuff.create()) {
		tcBuff.setUsagePattern(QGLBuffer::StaticDraw);
		tcBuff.bind();
		tcBuff.allocate(texCoord.data(), texCoord.count()*sizeof(QVector2D));
		indBuff.setUsagePattern(QGLBuffer::StaticDraw);
		indBuff.bind();
		indBuff.allocate(indices.data(), indices.count()*sizeof(unsigned short));
	}
}

Plate::Plate(const QRectF &geometry, float borderSize): Plate() {
	setGeometry(geometry, borderSize);
}

Plate::~Plate() {

}

void Plate::setBorderSize(float borderSize) {
	if (_borderSize == borderSize)
		return;

	_borderSize = borderSize;
	updateGeometry();
}

void Plate::setSize(const QSizeF &size) {
	if (_geometry.size() == size)
		return;

	_geometry.setSize(size);
	updateGeometry();
}

void Plate::setGeometry(const QRectF &geometry, float borderSize) {
	if (_geometry==geometry && (_borderSize==borderSize || borderSize==-1))
		return;

	_geometry = geometry;

	if (borderSize>=0)
		_borderSize = borderSize;

	updateGeometry();
}

void Plate::setGeometry(const QRectF &geometry, const QPointF &localPos, float borderSize) {
	QRectF localRect = geometry;
	localRect.moveTopLeft(localPos);
	setGeometry(localRect, borderSize);
}

void Plate::setPosition(const QPointF &position) {
	if (_geometry.topLeft() == position)
		return;

	_geometry.moveTopLeft(position);
	updateGeometry();
}

void Plate::setTheme(float theme) {
	_theme = theme;
}

void Plate::setMaterial(Material *material) {
	this->material = material;
}

void Plate::render() {
	glEnableVertexAttribArray(material->adrAtrVertex());
	glEnableVertexAttribArray(material->adrAtrTexCoord());

	if (tcBuff.isCreated() && indBuff.isCreated() && vertBuff.isCreated()) {
		vertBuff.bind();
		if (modified) {
			vertBuff.write(0, vertices.data(), vertices.count()*sizeof(QVector2D));
			modified = false;
		}
		glVertexAttribPointer(material->adrAtrVertex(), 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		tcBuff.bind();
		glVertexAttribPointer(material->adrAtrTexCoord(), 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		indBuff.bind();
		glDrawElements(GL_TRIANGLES, indices.count(), GL_UNSIGNED_SHORT, nullptr);

		vertBuff.release();
		tcBuff.release();
		indBuff.release();

	} else {
		glVertexAttribPointer(material->adrAtrVertex(), 2, GL_FLOAT, GL_FALSE, 0, vertices.data());
		glVertexAttribPointer(material->adrAtrTexCoord(), 2, GL_FLOAT, GL_FALSE, 0, texCoord.data());
		glDrawElements(GL_TRIANGLES, indices.count(), GL_UNSIGNED_SHORT, indices.data());
	}

	glDisableVertexAttribArray(material->adrAtrVertex());
	glDisableVertexAttribArray(material->adrAtrTexCoord());
}

void Plate::updateGeometry() {
	vertices[0 ] = {_geometry.left(), _geometry.top()};
	vertices[1 ] = {_geometry.left(), _geometry.top()+_borderSize};
	vertices[2 ] = {_geometry.left()+_borderSize, _geometry.top()+_borderSize};
	vertices[3 ] = {_geometry.left()+_borderSize, _geometry.top()};

	vertices[4 ] = {_geometry.right()-_borderSize, _geometry.top()};
	vertices[5 ] = {_geometry.right()-_borderSize, _geometry.top()+_borderSize};
	vertices[6 ] = {_geometry.right(), _geometry.top()+_borderSize};
	vertices[7 ] = {_geometry.right(), _geometry.top()};

	vertices[8 ] = {_geometry.left(), _geometry.bottom()-_borderSize};
	vertices[9 ] = {_geometry.left(), _geometry.bottom()};
	vertices[10] = {_geometry.left()+_borderSize, _geometry.bottom()};
	vertices[11] = {_geometry.left()+_borderSize,  _geometry.bottom()-_borderSize};

	vertices[12] = {_geometry.right()-_borderSize, _geometry.bottom()-_borderSize};
	vertices[13] = {_geometry.right()-_borderSize, _geometry.bottom()};
	vertices[14] = {_geometry.right(), _geometry.bottom()};
	vertices[15] = {_geometry.right(), _geometry.bottom()-_borderSize};

	modified = true;
}


/*
 *   [0]-----[3]---------[4]-----[7]
 *    |       |           |       |
 *    |   1   |     2     |   3   |
 *    |       |           |       |
 *   [1]-----[2]---------[5]-----[6]
 *    |       |           |       |
 *    |       |           |       |
 *    |   4   |     5     |   6   |
 *    |       |           |       |
 *    |       |           |       |
 *   [8]-----[11]--------[12]----[15]
 *    |       |           |       |
 *    |   7   |     8     |   9   |
 *    |       |           |       |
 *   [9]-----[10]--------[13]----[14]
 */
