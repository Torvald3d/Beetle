#include "quadmodel.h"
#include <QRectF>
#include "debug.h"

QuadModel::QuadModel(bool useVbo): QuadModel(nullptr, useVbo) {

}

QuadModel::QuadModel(Material *material, bool useVbo) {
	this->useVbo = useVbo;

	vertices.fill({0, 0}, 4);

	indices << 0 << 1 << 2 << 0 << 2 << 3;

	texCoord.append({0, 1});
	texCoord.append({0, 0});
	texCoord.append({1, 0});
	texCoord.append({1, 1});

	setMaterial(material);
}

QuadModel::QuadModel(const QRectF &rect, Material *material, bool useVbo): QuadModel(useVbo) {
	resize(rect);
	setMaterial(material);
}

QuadModel::~QuadModel() {

}

void QuadModel::updateGeometry() {
	vertices[0] = {_geometry.left(),  _geometry.top()};
	vertices[1] = {_geometry.left(),  _geometry.bottom()};
	vertices[2] = {_geometry.right(), _geometry.bottom()};
	vertices[3] = {_geometry.right(), _geometry.top()};

	if (useVbo)
		updateBuffers();
}

void QuadModel::resize(const QRectF &rect) {
	_geometry = rect;
	updateGeometry();
}

void QuadModel::setPosition(const QPointF &position) {
	_geometry.moveTopLeft(position);
	updateGeometry();
}

bool QuadModel::updateBuffers() {
	if (vertices.isEmpty() || indices.isEmpty() || texCoord.isEmpty()) {
		qDebug() << "Vertex, indices or texCoords array can't be empty" << __DEBUG_INFO__;
		return false;
	}

	if (!createBuffer(vertBuff, vertices))
		return false;

	if (!createBuffer(tcBuff, texCoord))
		return false;

	if (!createBuffer(indBuff, indices))
		return false;

	return true;
}

void QuadModel::render() {
	if (!material)
		return;

	glEnableVertexAttribArray(material->adrAtrVertex());
	if (material->adrAtrTexCoord()>=0)
		glEnableVertexAttribArray(material->adrAtrTexCoord());

	// if VBO is avalable and created
	if (vertBuff.isCreated() && indBuff.isCreated() && tcBuff.isCreated()
		&& material->adrAtrVertex()>=0 && useVbo)
	{
		vertBuff.bind();
		glVertexAttribPointer(material->adrAtrVertex(), 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		if (material->adrAtrTexCoord()>=0) {
			tcBuff.bind();
			glVertexAttribPointer(material->adrAtrTexCoord(), 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		}

		indBuff.bind();
		glDrawElements(GL_TRIANGLES, indices.count(), GL_UNSIGNED_INT, nullptr);

		vertBuff.release();
		tcBuff.release();
		indBuff.release();

	// if VBO unsupported or disabled
	} else {
		if (vertices.isEmpty() || indices.isEmpty() || texCoord.isEmpty()
			|| material->adrAtrVertex()<0)
			return;

		glVertexAttribPointer(material->adrAtrVertex(), 2, GL_FLOAT, GL_FALSE, 0, vertices.data());
		if (material->adrAtrTexCoord()>=0)
			glVertexAttribPointer(material->adrAtrTexCoord(), 2, GL_FLOAT, GL_FALSE, 0, texCoord.data());
		glDrawElements(GL_TRIANGLES, indices.count(), GL_UNSIGNED_INT, indices.data());
	}

	glDisableVertexAttribArray(material->adrAtrVertex());
	if (material->adrAtrTexCoord()>=0)
		glDisableVertexAttribArray(material->adrAtrTexCoord());
}
