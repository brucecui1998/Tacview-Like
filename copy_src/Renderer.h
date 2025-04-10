// Renderer.h
#ifndef RENDERER_H
#define RENDERER_H

#include "FlightData.h"
#include <QOpenGLFunctions>

class Renderer : protected QOpenGLFunctions {
public:
    Renderer();
    void render(const FlightData& flightData, int currentTimeFrame);

private:
    void drawObject(const ObjectState& state);
    // 其他渲染相关的成员和方法
};

#endif // RENDERER_H
