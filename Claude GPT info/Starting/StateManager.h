// StateManager.h
#pragma once

#include "ControlState.h"

/// @brief Class for managing control states
class StateManager {
public:
    /// @brief Constructor
    StateManager();

    /// @brief Update the gain state
    /// @param newGainState The new gain state
    void updateGainState(int newGainState);

    /// @brief Toggle the phase state
    void togglePhaseState();

    /// @brief Toggle the phantom power state
    void togglePhantomPowerState();

    /// @brief Toggle the input impedance state
    void toggleInputImpedanceState();

    /// @brief Toggle the pad state
    void togglePadState();

    /// @brief Toggle the high pass filter state
    void toggleHighPassFilterState();

    /// @brief Get the current control state
    /// @return The current control state
    ControlState getControlState() const;

    /// @brief Check if the state has changed
    /// @return True if the state has changed, false otherwise
    bool isStateChanged() const;

    /// @brief Reset the state changed flag
    void resetStateChanged();

private:
    ControlState _controlState;
    bool _stateChanged;
};