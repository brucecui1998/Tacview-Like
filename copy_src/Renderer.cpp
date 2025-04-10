// Renderer.cpp
#include "Renderer.h"

Renderer::Renderer() {
    initializeOpenGLFunctions();
    // 初始化 OpenGL 状态
}

void Renderer::render(const FlightData& flightData, int currentTimeFrame) {
    auto objects = flightData.getObjectStatesAt(currentTimeFrame);
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        drawObject(it.value());
    }
}

void Renderer::drawObject(const ObjectState& state) {
    // 使用 OpenGL 绘制对象
}
