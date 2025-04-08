#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "FlightData.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget();

    void setFlightData(const FlightData &data);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    FlightData flightData;
};