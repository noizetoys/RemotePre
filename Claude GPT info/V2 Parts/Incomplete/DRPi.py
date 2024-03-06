# drpi.py
import socket
import struct
import smbus
import logging
import configparser

# ... (constants and configuration remain the same)

control_states = {}

def receive_control_states(sock):
  try:
    data = sock.recv(I2C_BUFFER_SIZE)
    if len(data) == struct.calcsize(CONTROL_STATE_FORMAT):
      unpacked_data = struct.unpack(CONTROL_STATE_FORMAT, data)
      control_state = {
        'gain': unpacked_data[0],
        'phase': unpacked_data[1],
        'phantom_power': unpacked_data[2],
        'input_impedance': unpacked_data[3],
        'pad': unpacked_data[4],
        'high_pass_filter': unpacked_data[5],
        'mute': unpacked_data[6]
      }
      return control_state
  except Exception as e:
    logging.error(f"Error receiving control states: {str(e)}")
  return None

def send_control_states(control_state):
  try:
    packed_data = struct.pack(CONTROL_STATE_FORMAT,
                              control_state['gain'],
                              int(control_state['phase']),
                              int(control_state['phantom_power']),
                              int(control_state['input_impedance']),
                              int(control_state['pad']),
                              int(control_state['high_pass_filter']),
                              int(control_state['mute']))
    i2c_bus.write_i2c_block_data(I2C_DEVICE_ADDRESS, 0x00, list(packed_data))
    return True
  except Exception as e:
    logging.error(f"Error sending control states to device: {str(e)}")
  return False

def send_control_states_to_crpi(address, control_state):
  data_to_send = struct.pack('B' + CONTROL_STATE_FORMAT,
                             address,
                             control_state['gain'],
                             int(control_state['phase']),
                             int(control_state['phantom_power']),
                             int(control_state['input_impedance']),
                             int(control_state['pad']),
                             int(control_state['high_pass_filter']),
                             int(control_state['mute']))
  try:
    crpi_socket.sendall(data_to_send)
  except Exception as e:
    logging.error(f"Error sending control states to CRPi: {str(e)}")

def handle_crpi_connection(sock):
  while True:
    control_state = receive_control_states(sock)
    if control_state is not None:
      address = I2C_DEVICE_ADDRESS
      send_control_states(control_state)
      control_states[address] = control_state
    
    # Check for incoming data from DA
    for address in control_states:
      try:
        data = i2c_bus.read_i2c_block_data(address, 0x