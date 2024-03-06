// ControlState.h
#pragma once

/// @brief Structure representing the control state
struct ControlState {
    int gainLevel;
    bool phaseState;
    bool phantomPowerState;
    bool inputImpedanceState;
    bool padState;
    bool highPassFilterState;
};