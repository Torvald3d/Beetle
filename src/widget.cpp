#include "widget.h"
#include <memory>
#include "debug.h"
#include <QTime>
#include <QDir>
#include <QApplication>
#include <QDesktopWidget>
#include "utils/math.h"


/* ===============================================================================
 *                                   INIT
 * =============================================================================*/


GLWidget::GLWidget(QGLWidget *parent): QGLWidget(QGLFormat(QGL::DoubleBuffer), parent) {
	QGLFormat glFormat(QGL::DoubleBuffer);
	glFormat.setSwapInterval(0);
	setFormat(glFormat);
	connect(&fixedTimer, SIGNAL(timeout()), this, SLOT(fixedUpdate()));
}

GLWidget::~GLWidget() {
	delete gui;
	delete fpsWindow;

	delete readBuffer;
	delete writeBuffer;
	delete lowBuffer;
	delete guiFbo;
	delete fpsFbo;

	delete envShader;
	delete quadShader;
	delete quadAlphaShader;
	delete blurShader;
	delete guiShader;
	delete iridescenceShader;
	delete colorShader;
	delete colorNMShader;
	delete downsampleShader;
	delete fxaaShader;

	delete quadMesh;
	delete guiMesh;
	delete fpsMesh;
	delete model;
	delete stick;
	delete belly;
	delete eyes;

	delete envMaterial;
	delete quadMaterial;
	delete quadAlphaMaterial;
	delete iridescenceMaterial;
	delete bellyMaterial;
	delete stickMaterial;
	delete eyesMaterial;
	delete blurMaterial;
	delete guiMaterial;
	delete fpsMaterial;
	delete downsampleMaterial;
	delete fxaaMaterial;

	for (Texture *texture: guiThemes)
		delete texture;
}

void GLWidget::initializeGL() {
	GLenum err;

#ifndef Q_OS_MAC
	glewExperimental = true;
	err = glewInit();
	if (err)
		qCritical() << "Can't init glew:" << err << __DEBUG_INFO__;
#endif

	QTime time;
	time.start();

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glClearColor(0, 0, 0, 0);

	if (!initTextures()) {
		qCritical() << "Can't init textures" << __DEBUG_INFO__;
		return;
	}

	if (!initFrameBuffers()) {
		qCritical() << "Can't init FBO" << __DEBUG_INFO__;
		return;
	}

	envShader = initShader(shadersPath+"environment.vert", shadersPath+"environment.frag");
	quadShader = initShader(shadersPath+"quad.vert", shadersPath+"quad.frag");
	quadAlphaShader = initShader(shadersPath+"quad.vert", shadersPath+"quadAlpha.frag");
	blurShader = initShader(shadersPath+"quad.vert", shadersPath+"blur.frag");
	guiShader = initShader(shadersPath+"gui.vert", shadersPath+"gui.frag");
	iridescenceShader = initShader(shadersPath+"colornm.vert", shadersPath+"iridescence.frag");
	colorShader = initShader(shadersPath+"color.vert", shadersPath+"color.frag");
	colorNMShader = initShader(shadersPath+"colornm.vert", shadersPath+"colornm.frag");
	downsampleShader = initShader(shadersPath+"quad.vert", shadersPath+"downsample.frag");
	fxaaShader = initShader(shadersPath+"quad.vert", shadersPath+"fxaa.frag");

	if (!initMaterials()) {
		qCritical() << "Can't init materials" << __DEBUG_INFO__;
		return;
	}

	try {
		quadMesh = new QuadModel(QRectF(-1, 1, 2, -2));
		guiMesh = new QuadModel(false);
		fpsMesh = new QuadModel(false);
		model = new Model(modelPath+"model.model", iridescenceMaterial);
		stick = new Model(modelPath+"stick.model", stickMaterial);
		belly = new Model(modelPath+"belly.model", bellyMaterial);
		eyes = new Model(modelPath+"eyes.model", eyesMaterial);
	} catch(...) {
		qCritical() << "Can't init models" << __DEBUG_INFO__;
		return;
	}

	initGui();

	err = glGetError();
	if (err)
		qWarning() << "OpenGL error: " << err;

	qDebug() << "full init time:" << time.elapsed();

	timer.start(0, this);
	fixedTimer.start(16);
}

bool GLWidget::initTextures() {
	QDir envDir(hdrPath, "*.hdr");
	envPathes = envDir.entryList();
	if(envPathes.isEmpty()) {
		qCritical() << "Can't find HDR images in " + hdrPath << __DEBUG_INFO__;
		return false;
	}
	changeEnv();

	if (!guiMap.createTexture(guiPath+"map.png", GL_RED, GL_LINEAR, GL_CLAMP_TO_EDGE)) {
		qCritical() << "Can't load guiMap" << __DEBUG_INFO__;
		return false;
	}

	QDir themesDir(guiPath, "theme*");
	themeNames = themesDir.entryList();
	try {
		for (QString &name: themeNames)
			guiThemes.append(new Texture(guiPath+name, GL_RGBA, GL_LINEAR, GL_CLAMP_TO_EDGE));
	} catch(...) {
		qCritical() << "Can't create guiTheme textures" << __DEBUG_INFO__;
		return false;
	}

	if (guiThemes.isEmpty()) {
		qCritical() << "Can't find GUI themes in " + guiPath << __DEBUG_INFO__;
		return false;
	}

	if (!dataTexModel.createTexture(modelPath+"dataTexture.png", GL_RGBA,
									  GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE)) {
		qCritical() << "Can't load dataTexture" << __DEBUG_INFO__;
		return false;
	}

	int n = 128;
	unsigned char rnd[n];
	rnd[0] = rnd[1] = 0;
	for (int i=2; i<n; i++)
		rnd[i] = rand()%255;
	rndTexture.createTexture(GL_RG8, n, 1, GL_RG, GL_UNSIGNED_BYTE, GL_NEAREST,
							 GL_CLAMP_TO_EDGE, &rnd);

	return true;
}

bool GLWidget::initFrameBuffers() {
	try {
		readBuffer = new FrameBuffer(size(), 1);
		writeBuffer = new FrameBuffer(size(), 1);
		lowBuffer = new FrameBuffer(size(), 3);
		guiFbo = new FrameBuffer(QSize(1, 1), 1);
		fpsFbo = new FrameBuffer(QSize(1, 1), 1);
	} catch(...) {
		qCritical() << "Can't create FBO" << __DEBUG_INFO__;
		return false;
	}

	readBuffer->addRenderTarget(GL_RGBA16F_ARB, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR,
								GL_CLAMP_TO_EDGE, "color");
	readBuffer->addRenderTarget(GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT,
								 GL_NEAREST, GL_MIRRORED_REPEAT, "depth");
	if (!readBuffer->create())
		return false;

	writeBuffer->addRenderTarget(GL_RGBA16F_ARB, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR,
								 GL_CLAMP_TO_EDGE, "color");
	writeBuffer->addRenderTarget(GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT,
								 GL_NEAREST, GL_MIRRORED_REPEAT, "depth");
	if (!writeBuffer->create())
		return false;

	lowBuffer->addRenderTarget(GL_RGBA16F_ARB, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR,
								 GL_CLAMP_TO_EDGE, "color");
	lowBuffer->addRenderTarget(GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT,
								 GL_NEAREST, GL_MIRRORED_REPEAT, "depth");
	if (!lowBuffer->create())
		return false;

	guiFbo->addRenderTarget(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR,
							GL_CLAMP_TO_EDGE, "color");
	if (!guiFbo->create())
		return false;

	fpsFbo->addRenderTarget(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR,
							GL_CLAMP_TO_EDGE, "color");
	if (!fpsFbo->create())
		return false;

	return true;
}

QGLShaderProgram *GLWidget::initShader(const QString &vert, const QString &frag) {
	std::unique_ptr<QGLShaderProgram> shader(new(std::nothrow) QGLShaderProgram(this));
	if (shader==nullptr)
		qCritical() << "Can't init shader: " + vert + ", " + frag << __DEBUG_INFO__;

	if (!shader->addShaderFromSourceFile(QGLShader::Vertex, vert))
		qCritical() << "Can't init vertex shader: " + vert << __DEBUG_INFO__;
	if (!shader->addShaderFromSourceFile(QGLShader::Fragment, frag))
		qCritical() << "Can't init fragment shader: " + frag << __DEBUG_INFO__;
	if (!shader->link())
		qCritical() << "Can't link shader: " + vert + ", " + frag << __DEBUG_INFO__;

	return shader.release();
}

bool GLWidget::initMaterials() {
	try {
		envMaterial = new EnvMaterial(envShader);
		quadMaterial = new QuadMaterial(quadShader);
		quadAlphaMaterial = new QuadAlphaMaterial(quadAlphaShader);
		iridescenceMaterial = new ColorMaterial(iridescenceShader);
		stickMaterial = new ColorMaterial(colorShader);
		bellyMaterial = new ColorMaterial(iridescenceShader);
		eyesMaterial = new ColorMaterial(colorNMShader);
		blurMaterial = new BlurMaterial(blurShader);
		guiMaterial = new GuiMaterial(guiShader);
		fpsMaterial = new GuiMaterial(guiShader);
		downsampleMaterial = new DownsampleMaterial(downsampleShader);
		fxaaMaterial = new FxaaMaterial(fxaaShader);
	} catch(...) {
		qCritical() << "Can't init envMaterials" << __DEBUG_INFO__;
		return false;
	}

	envMaterial->setEnvMap(&envTex);
	envMaterial->setCamera(&mainCamera);

	blurMaterial->setBlurSize(blurSize);

	guiMaterial->setCamera(&guiCamera);
	guiMaterial->setGuiMap(&guiMap);
	guiMaterial->setThemeMap(guiThemes[currGuiTheme]);

	fpsMaterial->setCamera(&fpsCamera);
	fpsMaterial->setGuiMap(&guiMap);
	fpsMaterial->setThemeMap(guiThemes[currGuiTheme]);

	iridescenceMaterial->setCamera(&mainCamera);
	iridescenceMaterial->setLight(light);
	iridescenceMaterial->setTexture(&dataTexModel);
	iridescenceMaterial->setEnvMap(&envTex);

	bellyMaterial->setCamera(&mainCamera);
	bellyMaterial->setLight(light);
	bellyMaterial->setTexture(&dataTexModel);
	bellyMaterial->setEnvMap(&envTex);

	stickMaterial->setCamera(&mainCamera);
	stickMaterial->setLight(light);
	stickMaterial->setTexture(&dataTexModel);
	stickMaterial->setColor(QColor::fromRgbF(0.58, 0.66, 0.35));

	eyesMaterial->setCamera(&mainCamera);
	eyesMaterial->setLight(light);
	eyesMaterial->setTexture(&dataTexModel);
	eyesMaterial->setEnvMap(&envTex);

	return true;
}

void GLWidget::initGui() {
	Layout::setDpi(QApplication::desktop()->physicalDpiX());
	Layout *root = nullptr;
	Layout *row1, *row2;
	Button *btn;

	float border = 33;
	float space = -20;
	float size = 48;
	float labelSize = 150;
	try {
		root = new Layout(Layout::Vertical, space/3, {border+5, border}); {
			row1 = new Layout(Layout::Vertical, space, {0, 0}); {
				row1->addItem(new Label(Label::Left, "   Environment  texture:", {labelSize, size}));
				row2 = new Layout(Layout::Horizontal, space, {0, 0}); {
					btn = new Button("<", {size, size});
					connect(btn, SIGNAL(release()), this, SLOT(prevEnv()));
					row2->addItem(btn);
					envLabel = new Label(Label::Center, envPathes[currEnv], {labelSize, size});
					row2->addItem(envLabel);
					btn = new Button(">", {size, size});
					connect(btn, SIGNAL(release()), this, SLOT(nextEnv()));
					row2->addItem(btn);
				}
				row1->addItem(row2);
			}
			root->addItem(row1);

			row1 = new Layout(Layout::Vertical, space, {0, 0}); {
				row1->addItem(new Label(Label::Left, "   Environment  bright:", {labelSize, size}));
				row2 = new Layout(Layout::Horizontal, space, {0, 0}); {
					btn = new Button("<", {size, size});
					connect(btn, SIGNAL(release()), this, SLOT(decBright()));
					btn->enableRepeat(true);
					row2->addItem(btn);
					brightLabel = new Label(Label::Center, "", {labelSize, size});
					brightLabel->setText(QString::number(bright, 'f', 2));
					row2->addItem(brightLabel);
					btn = new Button(">", {size, size});
					connect(btn, SIGNAL(release()), this, SLOT(incBright()));
					btn->enableRepeat(true);
					row2->addItem(btn);
				}
				row1->addItem(row2);
			}
			root->addItem(row1);

			row1 = new Layout(Layout::Vertical, space, {0, 0}); {
				row1->addItem(new Label(Label::Left, "   Gui  theme:", {labelSize, size}));
				row2 = new Layout(Layout::Horizontal, space, {0, 0}); {
					btn = new Button("<", {size, size});
					connect(btn, SIGNAL(release()), this, SLOT(prevGuiTheme()));
					row2->addItem(btn);
					guiThemeLabel = new Label(Label::Center, themeNames[currGuiTheme], {labelSize, size});
					row2->addItem(guiThemeLabel);
					btn = new Button(">", {size, size});
					connect(btn, SIGNAL(release()), this, SLOT(nextGuiTheme()));
					row2->addItem(btn);
				}
				row1->addItem(row2);
			}
			root->addItem(row1);

			row1 = new Layout(Layout::Vertical, space, {0, 0}); {
				row1->addItem(new Label(Label::Left, "   Blur  size:", {labelSize, size}));
				row2 = new Layout(Layout::Horizontal, space, {0, 0}); {
					btn = new Button("<", {size, size});
					connect(btn, SIGNAL(release()), this, SLOT(decBlur()));
					btn->enableRepeat(true);
					btn->setRepeatDelay(30);
					row2->addItem(btn);
					blurLabel = new Label(Label::Center, QString::number(blurSize), {labelSize, size});
					row2->addItem(blurLabel);
					btn = new Button(">", {size, size});
					connect(btn, SIGNAL(release()), this, SLOT(incBlur()));
					btn->enableRepeat(true);
					btn->setRepeatDelay(30);
					row2->addItem(btn);
				}
				row1->addItem(row2);
			}
			root->addItem(row1);

			toggleBlurButton = new Button("Disable blur", {labelSize+size*2+space*2, size});
			connect(toggleBlurButton, SIGNAL(release()), this, SLOT(toggleBlur()));
			root->addItem(toggleBlurButton);

			toggleFxaaButton = new Button("Disable FXAA", {labelSize+size*2+space*2, size});
			connect(toggleFxaaButton, SIGNAL(release()), this, SLOT(toggleFxaa()));
			root->addItem(toggleFxaaButton);

			toggleFpsButton = new Button("Hide FPS", {labelSize+size*2+space*2, size});
			connect(toggleFpsButton, SIGNAL(release()), this, SLOT(toggleFps()));
			root->addItem(toggleFpsButton);

			row1 = new Layout(Layout::Vertical, space, {0, 0}); {
				row1->addItem(new Label(Label::Left, "   Hue average:", {labelSize, size}));
				row2 = new Layout(Layout::Horizontal, space, {0, 0}); {
					btn = new Button("<", {size, size});
					connect(btn, SIGNAL(release()), this, SLOT(decHueAverage()));
					btn->enableRepeat(true);
					btn->setRepeatDelay(30);
					row2->addItem(btn);
					hueAverageLabel = new Label(Label::Center, QString::number(hueAverage), {labelSize, size});
					row2->addItem(hueAverageLabel);
					btn = new Button(">", {size, size});
					connect(btn, SIGNAL(release()), this, SLOT(incHueAverage()));
					btn->enableRepeat(true);
					btn->setRepeatDelay(30);
					row2->addItem(btn);
				}
				row1->addItem(row2);
			}
			root->addItem(row1);

			row1 = new Layout(Layout::Vertical, space, {0, 0}); {
				row1->addItem(new Label(Label::Left, "   Hue range:", {labelSize, size}));
				row2 = new Layout(Layout::Horizontal, space, {0, 0}); {
					btn = new Button("<", {size, size});
					connect(btn, SIGNAL(release()), this, SLOT(decHueRange()));
					btn->enableRepeat(true);
					btn->setRepeatDelay(30);
					row2->addItem(btn);
					hueRangeLabel = new Label(Label::Center, QString::number(hueRange), {labelSize, size});
					row2->addItem(hueRangeLabel);
					btn = new Button(">", {size, size});
					connect(btn, SIGNAL(release()), this, SLOT(incHueRange()));
					btn->enableRepeat(true);
					btn->setRepeatDelay(30);
					row2->addItem(btn);
				}
				row1->addItem(row2);
			}
			root->addItem(row1);
		}
	} catch(...) {
		qCritical() << "Can't create GUI" << __DEBUG_INFO__;
	}

	gui = new(std::nothrow) Window(this);
	if (!gui) {
		delete root;
		qCritical() << "Can't create GUI" << __DEBUG_INFO__;
		return;
	}

	gui->setLayout(root);
	gui->setMaterial(guiMaterial);
	guiFbo->resize(gui->size().toSize());

	fpsLabel = new(std::nothrow) Label(Label::Center, "", {170, 100});
	fpsWindow = new(std::nothrow) Window(this);
	if (!fpsWindow || !fpsLabel) {
		qCritical() << "Can't create fpsWindow" << __DEBUG_INFO__;
		return;
	}

	fpsWindow->setLayout(fpsLabel);
	fpsWindow->setMaterial(fpsMaterial);
	fpsFbo->resize(fpsWindow->size().toSize());

	changeBright();
	changeBlurSize();
	changeDistance(0);
	changeHueAverage();
	changeHueRange();
}

void GLWidget::resizeGL(int, int) {
	gui->setPosition(QPoint(width()-gui->size().width(), 0));
	float w = 2*fpsWindow->size().width()/width();
	float h = 2*fpsWindow->size().height()/height();
	fpsMesh->resize(QRectF(-1, 1, w, -h));

	readBuffer->resize(size());
	writeBuffer->resize(size());

	mainCamera.initPerspective(30, static_cast<float>(width())/height(), 0.1, 50);
	mainCamera.setDistance(distance);
	guiCamera.initOrtho(0, gui->size().width(), gui->size().height(), 0, -1, 1);
	fpsCamera.initOrtho(0, fpsWindow->size().width(), fpsWindow->size().height(), 0, -1, 1);

	downsampleMaterial->setSize(size());
	downsampleMaterial->setAspect(mainCamera.aspect());
	blurMaterial->setScreenSize(size());
	fxaaMaterial->setScreenSize(size());
}


/* ===============================================================================
 *                                   RENDER
 * =============================================================================*/


void GLWidget::timerEvent(QTimerEvent *) {
	fps.tic();
	updateGL();
	if (fpsLabel->text()!=fps.getFps()) {
		fpsLabel->setText(fps.getFps());
		fpsWindow->update();
	}
}

void GLWidget::fixedUpdate() {
	QPoint cursorPos = mapFromGlobal(QCursor::pos());
	cursorPos.setX(Math::clamp(cursorPos.x(), 0, width()));
	cursorPos.setY(Math::clamp(cursorPos.y(), 0, height()));
	float fadeSpeed = time.delta()*4;
	if (showGui && (gui->contains(cursorPos) || clickGui))
		fadeSpeed = -fadeSpeed;
	guiAlpha = Math::clamp(guiAlpha-fadeSpeed, 0, 1);
	if (firstStart) { // without this hack renderText don't work first time >:(
		gui->update();
		firstStart = false;
	}

	time.tic();
}

void GLWidget::paintGL() {
	renderEnvironment();
	renderModels();
	if (enableFxaa)
		applayFxaa();
	if (enableBlur)
		renderBlur();
	renderGui();
	renderScreen();
}

void GLWidget::swapBuffers() {
	FrameBuffer *tmp = readBuffer;
	readBuffer = writeBuffer;
	writeBuffer = tmp;
}

void GLWidget::renderEnvironment() {
	writeBuffer->bind();
	envMaterial->bind();
	quadMesh->setMaterial(envMaterial);
	quadMesh->render();
}

void GLWidget::renderModels() {
	writeBuffer->bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	iridescenceMaterial->bind();
	model->render();
	stickMaterial->bind();
	stick->render();
	bellyMaterial->bind();
	belly->render();
	eyesMaterial->bind();
	eyes->render();
	glDisable(GL_DEPTH_TEST);
}

void GLWidget::applayFxaa() {
	swapBuffers();
	writeBuffer->bind();
	fxaaMaterial->setTexture(readBuffer->texture());
	fxaaMaterial->bind();
	quadMesh->setMaterial(fxaaMaterial);
	quadMesh->render();
}

void GLWidget::renderBlur() {
	swapBuffers();

	lowBuffer->bind();
	downsampleMaterial->setTexture(readBuffer->texture());
	downsampleMaterial->bind();
	quadMesh->setMaterial(downsampleMaterial);
	quadMesh->render();

	writeBuffer->bind();
	blurMaterial->setTexture(readBuffer->texture());
	blurMaterial->setLowTexture(lowBuffer->texture());
	blurMaterial->setSeed(Math::realRand(1, 2));
	blurMaterial->bind();
	quadMesh->setMaterial(blurMaterial);
	quadMesh->render();
}

void GLWidget::renderGui() {
	if (gui->needRender() && showGui) {
		guiFbo->bind();
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
		gui->render();
		quadMaterial->bind(); // without this hack renderText don't work :'(
		Material::unbind();
		gui->renderText();
		glDisable(GL_BLEND);
		float w = 2*gui->size().width()/width();
		float h = 2*gui->size().height()/height();
		guiMesh->resize(QRectF(1-w, 1, w, -h));
	}

	if (fpsWindow->needRender()) {
		fpsFbo->bind();
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
		fpsWindow->render();
		quadMaterial->bind();
		Material::unbind();
		fpsWindow->renderText();
		glDisable(GL_BLEND);
	}
}

void GLWidget::renderScreen() {
	FrameBuffer::unbind(size());
	swapBuffers();
	quadMaterial->setTexture(readBuffer->texture());
	quadMaterial->bind();
	quadMesh->setMaterial(quadMaterial);
	quadMesh->render();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	quadAlphaMaterial->setTexture(guiFbo->texture());
	quadAlphaMaterial->setAlpha(guiAlpha);
	quadAlphaMaterial->bind();
	guiMesh->setMaterial(quadAlphaMaterial);
	guiMesh->render();
	if (showFps) {
		quadAlphaMaterial->setTexture(fpsFbo->texture());
		quadAlphaMaterial->setAlpha(1);
		quadAlphaMaterial->bind();
		fpsMesh->setMaterial(quadAlphaMaterial);
		fpsMesh->render();
	}
	glDisable(GL_BLEND);
}


/* ===============================================================================
 *                                   CONTROL
 * =============================================================================*/


void GLWidget::keyPressEvent(QKeyEvent *e) {
	if (e->key()==Qt::Key_Escape)
		close();
}

void GLWidget::mousePressEvent(QMouseEvent *e) {
	gui->mouseChanged(e->pos(), MouseEvents::Press);

	if (gui->contains(e->pos())) {
		lastCursorPos.setX(-99999999);
		clickGui = true;
	} else {
		lastCursorPos = e->pos();
		showGui = false;
	}
}

void GLWidget::mouseMoveEvent(QMouseEvent *e) {
	gui->mouseChanged(e->pos(), MouseEvents::Move);

	if (lastCursorPos.x()<-999999)
		return;

	QPointF pos = e->pos()-lastCursorPos;
	if (e->buttons()==Qt::LeftButton)
		mainCamera.rotate(pos * -distance*0.02); // these magic numbers...
	if (e->buttons()==Qt::RightButton)
		changeDistance(pos.y()/500.0);
	lastCursorPos = e->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e) {
	gui->mouseChanged(e->pos(), MouseEvents::Release);
	showGui = true;
	clickGui = false;
}

void GLWidget::wheelEvent(QWheelEvent *e) {
	changeDistance(-e->delta()/1000.0);
}

void GLWidget::changeDistance(float value) {
	distance += 2;
	distance *= 1.0 + value;
	distance -= 2;
	if (distance>-2.2)
		distance = -2.2;
	if (distance<-25)
		distance = -25;
	mainCamera.setDistance(distance);

	focalDistance = distance+1;
	focalRange = -distance*0.75;

	iridescenceMaterial->setFocalDistance(focalDistance);
	iridescenceMaterial->setFocalRange(focalRange);
	bellyMaterial->setFocalDistance(focalDistance);
	bellyMaterial->setFocalRange(focalRange);
	stickMaterial->setFocalDistance(focalDistance);
	stickMaterial->setFocalRange(focalRange);
	eyesMaterial->setFocalDistance(focalDistance);
	eyesMaterial->setFocalRange(focalRange);
}

void GLWidget::nextEnv() {
	currEnv++;
	if (currEnv >= envPathes.length())
		currEnv = 0;
	changeEnv();
}

void GLWidget::prevEnv() {
	currEnv--;
	if (currEnv < 0)
		currEnv = envPathes.length()-1;
	changeEnv();
}

void GLWidget::changeEnv() {
	QTime time;
	time.start();

	HdrImage hdr(hdrPath+envPathes[currEnv], HdrImage::Format_RGB32F);
	light = hdr.lightDir();
	qDebug() << "lightDir" << light;
	if (!envTex.createHdrCubemap(hdr, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE))
		qWarning() << "Can't create cubemap from HDR image: " <<
					  envPathes[currEnv] << __DEBUG_INFO__;

	if (envLabel==nullptr)
		return;
	envLabel->setText(envPathes[currEnv]);
	gui->update();
	if (iridescenceMaterial)
		iridescenceMaterial->setLight(light);

	qDebug() << time.elapsed();
}

void GLWidget::changeBright() {
	envMaterial->setBright(bright);
	iridescenceMaterial->setBright(bright);
	stickMaterial->setBright(bright);
	bellyMaterial->setBright(bright);
	eyesMaterial->setBright(bright);
	brightLabel->setText(QString::number(bright, 'f', 2));
	gui->update();
}

void GLWidget::incBright() {
	bright += 0.05;
	if (bright>10)
		bright = 10;
	changeBright();
}

void GLWidget::decBright() {
	bright -= 0.05;
	if (bright<0.1)
		bright = 0.1;
	changeBright();
}

void GLWidget::changeGuiTheme() {
	guiMaterial->setThemeMap(guiThemes[currGuiTheme]);
	fpsMaterial->setThemeMap(guiThemes[currGuiTheme]);
	fpsWindow->update();
	guiThemeLabel->setText(themeNames[currGuiTheme]);
	gui->update();
}

void GLWidget::prevGuiTheme() {
	currGuiTheme--;
	if (currGuiTheme < 0)
		currGuiTheme = guiThemes.count()-1;
	changeGuiTheme();
}

void GLWidget::nextGuiTheme() {
	currGuiTheme++;
	if (currGuiTheme >= guiThemes.count())
		currGuiTheme = 0;
	changeGuiTheme();
}

void GLWidget::changeBlurSize() {
	blurMaterial->setBlurSize(blurSize);
	blurLabel->setText(QString::number(blurSize, 'f', 2));
	gui->update();
}

void GLWidget::incBlur() {
	blurSize += 0.25;
	changeBlurSize();
}

void GLWidget::decBlur() {
	if (blurSize>0)
		blurSize -= 0.25;
	if (blurSize<1)
		blurSize = 1;
	changeBlurSize();
}

void GLWidget::toggleBlur() {
	enableBlur = !enableBlur;
	if (enableBlur)
		toggleBlurButton->setText("Disable blur");
	else
		toggleBlurButton->setText("Enable blur");
	gui->update();
}

void GLWidget::toggleFxaa() {
	enableFxaa = !enableFxaa;
	if (enableFxaa)
		toggleFxaaButton->setText("Disable FXAA");
	else
		toggleFxaaButton->setText("Enable FXAA");
	gui->update();
}

void GLWidget::toggleFps() {
	showFps = !showFps;
	if (showFps)
		toggleFpsButton->setText("Hide FPS");
	else
		toggleFpsButton->setText("Show FPS");
	gui->update();
}

void GLWidget::incHueAverage() {
	hueAverage += 0.01;
	if (hueAverage>0.999)
		hueAverage = 0.0;
	changeHueAverage();
}

void GLWidget::decHueAverage() {
	hueAverage -= 0.01;
	if (hueAverage<0.0)
		hueAverage = 1.0;
	changeHueAverage();
}

void GLWidget::incHueRange() {
	hueRange += 0.01;
	if (hueRange>1.0)
		hueRange = 1.0;
	changeHueRange();
}

void GLWidget::decHueRange() {
	hueRange -= 0.01;
	if (hueRange<0.0)
		hueRange = 0.0;
	changeHueRange();
}

void GLWidget::changeHueAverage() {
	iridescenceMaterial->setHueAverage(hueAverage);
	bellyMaterial->setHueAverage(hueAverage);
	hueAverageLabel->setText(QString::number(hueAverage, 'f', 2));
	gui->update();
}

void GLWidget::changeHueRange() {
	iridescenceMaterial->setHueRange(hueRange);
	bellyMaterial->setHueRange(hueRange);
	hueRangeLabel->setText(QString::number(hueRange, 'f', 2));
	gui->update();
}
