#ifndef WIDGET_H
#define WIDGET_H

#include <qglobal.h>
#ifndef Q_OS_MAC
	#include <GL/glew.h>
#endif
#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QBasicTimer>
#include <QTimer>

#include "utils/fps.h"
#include "utils/time.h"
#include "core/framebuffer.h"
#include "core/texture.h"
#include "core/camera.h"
#include "materials/envmaterial.h"
#include "materials/blurmaterial.h"
#include "materials/quadalphamaterial.h"
#include "materials/colormaterial.h"
#include "materials/downsamplematerial.h"
#include "materials/fxaamaterial.h"
#include "models/quadmodel.h"
#include "gui/window.h"

class GLWidget: public QGLWidget {
Q_OBJECT
public:
	GLWidget(QGLWidget *parent = nullptr);
	~GLWidget();

protected:
	virtual void initializeGL() override;
	virtual void resizeGL(int, int) override;
	virtual void paintGL() override;
	virtual void timerEvent(QTimerEvent *) override;

private slots:
	virtual void mousePressEvent(QMouseEvent *) override;
	virtual void mouseMoveEvent(QMouseEvent *) override;
	virtual void mouseReleaseEvent(QMouseEvent *) override;
	virtual void wheelEvent(QWheelEvent *) override;
	virtual void keyPressEvent(QKeyEvent *) override;

	void fixedUpdate();
	void nextEnv();
	void prevEnv();
	void incBright();
	void decBright();
	void nextGuiTheme();
	void prevGuiTheme();
	void incBlur();
	void decBlur();
	void toggleBlur();
	void toggleFxaa();
	void toggleFps();
	void incHueAverage();
	void decHueAverage();
	void incHueRange();
	void decHueRange();

private:
	// init

	const QString shadersPath = "shaders/";
	const QString resPath = "resource/";
	const QString hdrPath = resPath+"hdr/";
	const QString guiPath = resPath+"gui/";
	const QString modelPath = resPath+"model/";

	Window *gui = nullptr;
	Label *envLabel = nullptr;
	Label *brightLabel = nullptr;
	Label *guiThemeLabel = nullptr;
	Label *blurLabel = nullptr;
	Button *toggleBlurButton = nullptr;
	Button *toggleFxaaButton = nullptr;
	Window *fpsWindow = nullptr;
	Label *fpsLabel = nullptr;
	Button *toggleFpsButton = nullptr;
	Label *hueAverageLabel = nullptr;
	Label *hueRangeLabel = nullptr;

	QStringList envPathes;
	QStringList themeNames;
	Texture envTex;
	Texture guiMap;
	QList<Texture *> guiThemes;
	Texture dataTexModel;
	Texture rndTexture;

	Camera mainCamera;
	Camera guiCamera;
	Camera fpsCamera;

	FrameBuffer *readBuffer = nullptr;
	FrameBuffer *writeBuffer = nullptr;
	FrameBuffer *lowBuffer = nullptr;
	FrameBuffer *guiFbo = nullptr;
	FrameBuffer *fpsFbo = nullptr;

	QGLShaderProgram *envShader = nullptr;
	QGLShaderProgram *quadShader = nullptr;
	QGLShaderProgram *quadAlphaShader = nullptr;
	QGLShaderProgram *blurShader = nullptr;
	QGLShaderProgram *guiShader = nullptr;
	QGLShaderProgram *iridescenceShader = nullptr;
	QGLShaderProgram *colorShader = nullptr;
	QGLShaderProgram *colorNMShader = nullptr;
	QGLShaderProgram *downsampleShader = nullptr;
	QGLShaderProgram *fxaaShader = nullptr;

	QuadModel *quadMesh = nullptr;
	QuadModel *guiMesh = nullptr;
	QuadModel *fpsMesh = nullptr;
	Model *model = nullptr;
	Model *stick = nullptr;
	Model *belly = nullptr;
	Model *eyes = nullptr;

	EnvMaterial *envMaterial = nullptr;
	QuadMaterial *quadMaterial = nullptr;
	QuadAlphaMaterial *quadAlphaMaterial = nullptr;
	ColorMaterial *iridescenceMaterial = nullptr;
	ColorMaterial *bellyMaterial = nullptr;
	ColorMaterial *stickMaterial = nullptr;
	ColorMaterial *eyesMaterial = nullptr;
	BlurMaterial *blurMaterial = nullptr;
	GuiMaterial *guiMaterial = nullptr;
	GuiMaterial *fpsMaterial = nullptr;
	DownsampleMaterial *downsampleMaterial = nullptr;
	FxaaMaterial *fxaaMaterial = nullptr;

	bool initTextures();
	bool initFrameBuffers();
	QGLShaderProgram *initShader(const QString &vert, const QString &frag);
	bool initMaterials();
	void initGui();

	// render

	Fps fps;
	Time time;
	QBasicTimer timer;
	QTimer fixedTimer;
	float guiAlpha = 0;

	void swapBuffers();
	void renderEnvironment();
	void renderModels();
	void applayFxaa();
	void renderBlur();
	void renderGui();
	void renderScreen();

	// controlls

	float hueAverage = 0.11;
	float hueRange = 0.35;
	float distance = -10;
	QPoint lastCursorPos;
	float focalDistance = distance;
	float focalRange = 5;
	float blurSize = 4;
	QVector3D light;
	float bright = 1.5;
	int currGuiTheme = 0;
	int currEnv = 0;
	bool enableFxaa = true;
	bool showGui = true;
	bool showFps = true;
	bool clickGui = false;
	bool firstStart = true;
	bool enableBlur = true;

	void changeDistance(float value);
	void changeBlurSize();
	void changeEnv();
	void changeBright();
	void changeGuiTheme();
	void changeHueAverage();
	void changeHueRange();

};

#endif // WIDGET_H
