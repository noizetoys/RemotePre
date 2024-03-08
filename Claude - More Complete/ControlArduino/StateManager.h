// StateManager.h
#pragma once

#include "ControlState.h"

class StateManager {
public:
  StateManager();

  void updateGainState(int newGainState);
  void togglePhaseState();
  void togglePhantomPowerState();
  void toggleInputImpedanceState();
  void togglePadState();
  void toggleHighPassFilterState();
  void toggleMuteState();
  void updateControlState(const ControlState& newState);

  ControlState getControlState() const;
  bool isStateChanged() const;
  void resetStateChanged();

private:
  ControlState controlState;
  bool stateChanged;
};

// StateManager.cpp
#include "StateManager.h"

StateManager::StateManager() : stateChanged(false) {}

void StateManager::updateGainState(int newGainState) {
  if (controlState.gainLevel != newGainState) {
    controlState.gainLevel = newGainState;
    stateChanged = true;
  }
}

void StateManager::togglePhaseState() {
  controlState.phaseState = !controlState.phaseState;
  stateChanged = true;
}

void StateManager::togglePhantomPowerState() {
  controlState.phantomPowerState = !controlState.phantomPowerState;
  stateChanged = true;
}

void StateManager::toggleInputImpedanceState() {
  controlState.inputImpedanceState = !controlState.inputImpedanceState;
  stateChanged = true;
}

void StateManager::togglePadState() {
  controlState.padState = !controlState.padState;
  stateChanged = true;
}

void StateManager::toggleHighPassFilterState() {
  controlState.highPassFilterState = !controlState.highPassFilterState;
  stateChanged = true;
}

void StateManager::toggleMuteState() {
  controlState.muteState = !controlState.muteState;
  stateChanged = true;
}

void StateManager::updateControlState(const ControlState& newState) {
  controlState = newState;
  stateChanged = true;
}

ControlState StateManager::getControlState() const {
  return controlState;
}

bool StateManager::isStateChanged() const {
  return stateChanged;
}

void StateManager::resetStateChanged() {
  stateChanged = false;
}