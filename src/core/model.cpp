#include "model.h"
#include <QFile>
#include <QTime>
#include "debug.h"

template<typename T> bool AbstractModel::createBuffer(QGLBuffer &buff, QVector<T> &vec) {
	if (buff.isCreated()) {
		buff.bind();
		buff.allocate(vec.data(), vec.count()*sizeof(T));
		return true;
	}

	if (!buff.create()) {
		qWarning() << "Can't create VBO" << __DEBUG_INFO__;
		return false;
	}
	if (!buff.bind()) {
		buff.destroy();
		qWarning() << "Can't bind VBO" << __DEBUG_INFO__;
		return false;
	}

	buff.setUsagePattern(QGLBuffer::StaticDraw);
	buff.allocate(vec.data(), vec.count()*sizeof(T));
	return true;
}

void AbstractModel::setMaterial(Material *material) {
	this->material = material;
}

Model::Model() {

}

Model::Model(Material *material) {
	setMaterial(material);
}

Model::Model(const QString &path, Material *material) {
	setMaterial(material);
	if (!load(path))
		throw 1;
}

Model::~Model() {

}

bool Model::updateBuffers() {
	if (vertices.isEmpty() || indices.isEmpty()) {
		qDebug() << "Vertex and indices array can't be empty" << __DEBUG_INFO__;
		return false;
	}

	if (!createBuffer(vertBuff, vertices))
		return false;

	if (!normals.isEmpty())
		if (!createBuffer(normBuff, normals))
			return false;

	if (!tangetns.isEmpty())
		if (!createBuffer(tangBuff, tangetns))
			return false;

	if (!texCoord.isEmpty())
		if (!createBuffer(tcBuff, texCoord))
			return false;

	if (!createBuffer(indBuff, indices))
		return false;

	return true;
}

void Model::render() {
	if (!material)
		return;

	glEnableVertexAttribArray(material->adrAtrVertex());
	if (!normals.isEmpty() && material->adrAtrNormal()>=0)
		glEnableVertexAttribArray(material->adrAtrNormal());
	if (!tangetns.isEmpty() && material->adrAtrTangent()>=0)
		glEnableVertexAttribArray(material->adrAtrTangent());
	if (!texCoord.isEmpty() && material->adrAtrTexCoord()>=0)
		glEnableVertexAttribArray(material->adrAtrTexCoord());

	// if VBO is avalable and created
	if (vertBuff.isCreated() && indBuff.isCreated() && material->adrAtrVertex()>=0) {
		vertBuff.bind();
		glVertexAttribPointer(material->adrAtrVertex(), 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		if (!normals.isEmpty() && material->adrAtrNormal()>=0) {
			normBuff.bind();
			glVertexAttribPointer(material->adrAtrNormal(), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		}

		if (!tangetns.isEmpty() && material->adrAtrTangent()>=0) {
			tangBuff.bind();
			glVertexAttribPointer(material->adrAtrTangent(), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		}

		if (!texCoord.isEmpty() && material->adrAtrTexCoord()>=0) {
			tcBuff.bind();
			glVertexAttribPointer(material->adrAtrTexCoord(), 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		}

		indBuff.bind();
		glDrawElements(GL_TRIANGLES, indices.count(), GL_UNSIGNED_INT, nullptr);

		indBuff.release();
		vertBuff.release();
		normBuff.release();
		tangBuff.release();
		tcBuff.release();

	// if VBO unsupported or disabled
	} else {
		if (vertices.isEmpty() || indices.isEmpty() || material->adrAtrVertex()<0)
			return;

		glVertexAttribPointer(material->adrAtrVertex(), 3, GL_FLOAT, GL_FALSE, 0, vertices.data());

		if (!normals.isEmpty() && material->adrAtrNormal()>=0)
			glVertexAttribPointer(material->adrAtrNormal(), 3, GL_FLOAT, GL_FALSE, 0, normals.data());
		if (!tangetns.isEmpty() && material->adrAtrTangent()>=0)
			glVertexAttribPointer(material->adrAtrTangent(), 3, GL_FLOAT, GL_FALSE, 0, tangetns.data());
		if (!texCoord.isEmpty() && material->adrAtrTexCoord()>=0)
			glVertexAttribPointer(material->adrAtrTexCoord(), 2, GL_FLOAT, GL_FALSE, 0, texCoord.data());

		glDrawElements(GL_TRIANGLES, indices.count(), GL_UNSIGNED_INT, indices.data());
	}

	glDisableVertexAttribArray(material->adrAtrVertex());
	if (!normals.isEmpty() && material->adrAtrNormal()>=0)
		glDisableVertexAttribArray(material->adrAtrNormal());
	if (!tangetns.isEmpty() && material->adrAtrTangent()>=0)
		glDisableVertexAttribArray(material->adrAtrTangent());
	if (!texCoord.isEmpty() && material->adrAtrTexCoord()>=0)
		glDisableVertexAttribArray(material->adrAtrTexCoord());
}

bool Model::load(const QString &path) {
	QTime time;
	time.start();
	QFile file(path);

	if (!file.open(QIODevice::ReadOnly)) {
		qWarning() << "Can't open model: " + path << __DEBUG_INFO__;
		return false;
	}

	char *data = new char[file.size()];
	file.read(data, file.size());

	int count = ((int*)data)[1];
	int shift = ((int*)data)[2];
	vertices.resize(count);
	qCopy((QVector3D*)&data[shift], ((QVector3D*)&data[shift]+count), vertices.begin());

	count = ((int*)data)[3];
	shift = ((int*)data)[4];
	normals.resize(count);
	qCopy((QVector3D*)&data[shift], ((QVector3D*)&data[shift]+count), normals.begin());

	count = ((int*)data)[5];
	shift = ((int*)data)[6];
	tangetns.resize(count);
	qCopy((QVector3D*)&data[shift], ((QVector3D*)&data[shift]+count), tangetns.begin());

	count = ((int*)data)[7];
	shift = ((int*)data)[8];
	texCoord.resize(count);
	qCopy((QVector2D*)&data[shift], ((QVector2D*)&data[shift]+count), texCoord.begin());

	count = 3*((int*)data)[9];
	shift = ((int*)data)[10];
	indices.resize(count);
	qCopy((int*)&data[shift], ((int*)&data[shift]+count), indices.begin());

	qDebug() << "model" << path << "loading time:" << time.elapsed();
	file.close();
	delete[] data;
	return true;
}
