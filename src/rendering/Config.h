#pragma once

// ✅ 地球半径（真实地球，单位 km）
constexpr double EARTH_RADIUS = 6371.0;

// ✅ 飞机高度放大系数（视觉增强用）
constexpr double HEIGHT_EXAGGERATION = 150000.0;

// ✅ 飞机模型缩放比例（适配 km 尺度）
constexpr float AIRCRAFT_SCALE = 150.0f;  // 例如 150 米大小的飞机

// ✅ 摄像机默认距离（用于 km 尺度视角）
constexpr float CAMERA_MIN_DISTANCE = -100000.0f;
constexpr float CAMERA_MAX_DISTANCE = EARTH_RADIUS * 2.0f;
constexpr float CAMERA_DEFAULT_DISTANCE = EARTH_RADIUS * 2.0f;

