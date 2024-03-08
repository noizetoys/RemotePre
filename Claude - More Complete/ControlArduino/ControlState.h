// ControlState.h
#pragma once

struct ControlState {
  int gainLevel;
  bool phaseState;
  bool phantomPowerState;
  bool inputImpedanceState;
  bool padState;
  bool highPassFilterState;
  bool muteState;

  ControlState()
    : gainLevel(0),
      phaseState(false),
      phantomPowerState(false),
      inputImpedanceState(false),
      padState(false),
      highPassFilterState(false),
      muteState(false) {}

  bool operator==(const ControlState& other) const {
    return (
      gainLevel == other.gainLevel &&
      phaseState == other.phaseState &&
      phantomPowerState == other.phantomPowerState &&
      inputImpedanceState == other.inputImpedanceState &&
      padState == other.padState &&
      highPassFilterState == other.highPassFilterState &&
      muteState == other.muteState
    );
  }

  bool operator!=(const ControlState& other) const {
    return !(*this == other);
  }
};