#include "meshview.h"

#include "math.h"

/**
 * @brief MainView::MainView Creates a new main view.
 * @param parent QT parent widget.
 */
MeshView::MeshView(QWidget *parent) : QOpenGLWidget(parent) {
  qDebug() << "✓✓ MainView constructor";
}

/**
 * @brief MainView::~MainView Deconstructs the main view.
 */
MeshView::~MeshView() {
  debugLogger->stopLogging();
  clearArrays();

  // Clean up the buffers
  glDeleteBuffers(1, &fanCoordsBO);
  glDeleteBuffers(1, &fanColourBO);
  glDeleteBuffers(1, &fanIndexBO);
  glDeleteVertexArrays(1, &fanVAO);

  delete mainShaderProg;
  delete debugLogger;
  qDebug() << "✗✗ MainView destructor";
}

// ---

/**
 * @brief MainView::createShaderPrograms Creates the shader program used by this
 * application.
 */
void MeshView::createShaderPrograms() {
  // Qt approach
  mainShaderProg = new QOpenGLShaderProgram();
  mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                          ":/shaders/flat.vert");
  mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                          ":/shaders/flat.frag");

  mainShaderProg->link();

  // Qt wrappers:
  uniModelViewMatrix = mainShaderProg->uniformLocation("modelviewmatrix");
  uniProjectionMatrix = mainShaderProg->uniformLocation("projectionmatrix");

  // Pure OpenGL
  //  uniModelViewMatrix =
  //      glGetUniformLocation(mainShaderProg->programId(), "modelviewmatrix");
  //  uniProjectionMatrix =
  //      glGetUniformLocation(mainShaderProg->programId(), "projectionmatrix");
}

/**
 * @brief MainView::initBuffers Initialises the buffers.
 */
void MeshView::initBuffers() {
  // Pure OpenGL
  glGenVertexArrays(1, &fanVAO);
  glBindVertexArray(fanVAO);

  glGenBuffers(1, &fanCoordsBO);
  glBindBuffer(GL_ARRAY_BUFFER, fanCoordsBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

  glGenBuffers(1, &fanColourBO);
  glBindBuffer(GL_ARRAY_BUFFER, fanColourBO);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  glGenBuffers(1, &fanIndexBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fanIndexBO);

  glBindVertexArray(0);
}

/**
 * @brief MainView::updateBuffers Updates the n-gon with a new value for n.
 * @param n How many points the n-gon should have.
 */
void MeshView::updateBuffers(unsigned short n) {
  unsigned short k;
  float length = 0.6f;
  float tau = 6.283185f;

  clearArrays();

  triaCoords.reserve(n + 1);
  triaColours.reserve(n + 1);
  triaIndices.reserve(n + 2);

  triaCoords.append(QVector2D(0.0, 0.0));
  triaColours.append(convertHSLtoRGB(0.0, 1.0, 1.0));
  triaIndices.append(0);

  for (k = 0; k < n; k++) {
    float kn = static_cast<float>(k) / n;
    triaCoords.append(
        QVector2D(length * cos(tau * kn), length * sin(tau * kn)));
    triaColours.append(convertHSLtoRGB(kn, 0.8f, 0.5));

    triaIndices.append(k + 1);
  }

  triaIndices.append(1);

  glBindBuffer(GL_ARRAY_BUFFER, fanCoordsBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D) * triaCoords.size(),
               triaCoords.data(), GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, fanColourBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D) * triaColours.size(),
               triaColours.data(), GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fanIndexBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(unsigned short) * triaIndices.size(), triaIndices.data(),
               GL_DYNAMIC_DRAW);
}

void MeshView::updateMatrices() {
  modelViewMatrix.setToIdentity();

  projectionMatrix.setToIdentity();
  projectionMatrix.perspective(80.0, (float)width() / height(), 0.2, 2.0);

  updateUniformsRequired = true;
}

/**
 * @brief MainView::updateUniforms Updates the uniform. In this case the
 * modelview and the projection matrices used in the shaders.
 */
void MeshView::updateUniforms() {
  // Qt wrappers
  // mainShaderProg->setUniformValue(uniModelViewMatrix, modelViewMatrix);
  // mainShaderProg->setUniformValue(uniProjectionMatrix, projectionMatrix);

  // Pure OpenGL
  glUniformMatrix4fv(uniModelViewMatrix, 1, false, modelViewMatrix.data());
  glUniformMatrix4fv(uniProjectionMatrix, 1, false, projectionMatrix.data());

  updateUniformsRequired = false;
}

/**
 * @brief MainView::clearArrays Clears the data of the n-gon.
 */
void MeshView::clearArrays() {
  triaCoords.clear();
  triaCoords.squeeze();
  triaColours.clear();
  triaColours.squeeze();
  triaIndices.clear();
  triaIndices.squeeze();
}

// ---

/**
 * @brief MainView::convertHSLtoRGB Converts from HSL color space to RGB color
 * space.
 * @param H Hue.
 * @param S Saturation.
 * @param L Lightness.
 * @return RGB vector representation.
 */
QVector3D MeshView::convertHSLtoRGB(float H, float S, float L) {
  // Grey values, S=0.0, so C=0.0
  float C = (1.0f - abs(2.0f * L - 1.0f)) * S;
  float m = L - (C / 2.0f);
  float Hp = (H * 360.0f) / 60.0f;
  float X = C * (1.0f - abs(fmod(Hp, 2.0f) - 1.0f));

  QVector3D preRGB;
  if (Hp > 5.0f) {
    preRGB = QVector3D(C, 0.0, X);
  } else if (Hp > 4.0f) {
    preRGB = QVector3D(X, 0.0, C);
  } else if (Hp > 3.0f) {
    preRGB = QVector3D(0.0, X, C);
  } else if (Hp > 2.0f) {
    preRGB = QVector3D(0.0, C, X);
  } else if (Hp > 1.0f) {
    preRGB = QVector3D(X, C, 0.0);
  } else if (Hp >= 0.0f) {
    preRGB = QVector3D(C, X, 0.0);
  }

  return preRGB + QVector3D(m, m, m);
}

// ---

/**
 * @brief MainView::initializeGL Initialises the OpenGL context.
 */
void MeshView::initializeGL() {
  qDebug() << ":: Initializing OpenGL";
  initializeOpenGLFunctions();

  debugLogger = new QOpenGLDebugLogger();
  connect(debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this,
          SLOT(onMessageLogged(QOpenGLDebugMessage)), Qt::DirectConnection);

  if (debugLogger->initialize()) {
    qDebug() << ":: Logging initialized";
    debugLogger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
    debugLogger->enableMessages();
  }

  updateUniformsRequired = true;

  QString glVersion;
  glVersion = reinterpret_cast<const char *>(glGetString(GL_VERSION));
  qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

  // Enable depth buffer
  glEnable(GL_DEPTH_TEST);
  // Default is GL_LESS
  glDepthFunc(GL_LEQUAL);

  glPointSize(16);

  createShaderPrograms();
  initBuffers();
  updateBuffers(6);
  updateMatrices();
}

/**
 * @brief MainView::resizeGL Handles resizing of the window.
 * @param newWidth New width in pixels.
 * @param newHeight New height in pixels.
 */
void MeshView::resizeGL(int newWidth, int newHeight) { updateMatrices(); }

/**
 * @brief MainView::paintGL Draw call. Paints the n-gon on the screen.
 */
void MeshView::paintGL() {
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mainShaderProg->bind();

  if (updateUniformsRequired) {
    updateUniforms();
  }

  glBindVertexArray(fanVAO);

  glDrawElements(GL_TRIANGLE_FAN, triaIndices.size(), GL_UNSIGNED_SHORT,
                 nullptr);
  glDrawElements(GL_POINTS, triaIndices.size() - 2, GL_UNSIGNED_SHORT,
                 (GLvoid *)(sizeof(GLushort)));

  glBindVertexArray(0);

  mainShaderProg->release();
}

/**
 * @brief MainView::onMessageLogged Helper function for logging messages.
 * @param message The message to log.
 */
void MeshView::onMessageLogged(QOpenGLDebugMessage Message) {
  qDebug() << " → Log:" << Message;
}
