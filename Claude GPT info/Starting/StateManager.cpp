// StateManager.cpp
#include "StateManager.h"

/// @brief Constructor
StateManager::StateManager()
    : _controlState{0, false, false, false, false, false}, _stateChanged(false) {}

/// @brief Update the gain state
/// @param newGainState The new gain state
void StateManager::updateGainState(int newGainState) {
    if (_controlState.gainLevel != newGainState) {
        _controlState.gainLevel = newGainState;
        _stateChanged = true;
    }
}

/// @brief Toggle the phase state
void StateManager::togglePhaseState() {
    _controlState.phaseState = !_controlState.phaseState;
    _stateChanged = true;
}

/// @brief Toggle the phantom power state
void StateManager::togglePhantomPowerState() {
    _controlState.phantomPowerState = !_controlState.phantomPowerState;
    _stateChanged = true;
}

/// @brief Toggle the input impedance state
void StateManager::toggleInputImpedanceState() {
    _controlState.inputImpedanceState = !_controlState.inputImpedanceState;
    _stateChanged = true;
}

/// @brief Toggle the pad state
void StateManager::togglePadState() {
    _controlState.padState = !_controlState.padState;
    _stateChanged = true;
}

/// @brief Toggle the high pass filter state
void StateManager::toggleHighPassFilterState() {
    _controlState.highPassFilterState = !_controlState.highPassFilterState;
    _stateChanged = true;
}

/// @brief Get the current control state
/// @return The current control state
ControlState StateManager::getControlState() const {
    return _controlState;
}

/// @brief Check if the state has changed
/// @return True if the state has changed, false otherwise
bool StateManager::isStateChanged() const {
    return _stateChanged;
}

/// @brief Reset the state changed flag
void StateManager::resetStateChanged() {
    _stateChanged = false;
}