#include "SceneManager.h"

void SceneManager::bindDataProvider(IFlightDataProvider* provider) {
    dataProvider = provider;
}

void SceneManager::updateCurrentTime(double time) {
    currentTime = time;
}

QMap<QString, ObjectState> SceneManager::getCurrentObjectStates() const {
    if (!dataProvider) return {};
    return dataProvider->getObjectStatesAt(currentTime);
}

QList<double> SceneManager::getTimeFrames() const {
    if (!dataProvider) return {};
    return dataProvider->getAllTimeFrames();
}

double SceneManager::getCurrentTime() const {
    return currentTime;
}
