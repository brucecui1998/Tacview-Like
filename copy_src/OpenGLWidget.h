#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QTimer>
#include <QElapsedTimer>
#include "FlightData.h"
#include "EarthSphere.h"
#include "MouseCameraController.h"
#include "ObjModel.h"


class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget();

    void setFlightData(const FlightData &data);

public slots:
    void startPlayback();  // 槽函数，控制开始播放

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;


private:
    FlightData flightData;
    EarthSphere earth;
    MouseCameraController cameraController;
    ObjModel f16Model;

    QTimer playbackTimer;            // 播放用的定时器（已有）
    QElapsedTimer playbackClock;     // ✅ 用于测量真实经过时间
    QList<double> frameTimes;        // ✅ 所有 ACMI 时间帧
    int frameIndex = 0;              // ✅ 当前帧索引
    double currentTime = 0.0;        // ✅ 当前播放时间（用于渲染）


    void drawCompass();  // 🔴 渲染右上角指南针
};