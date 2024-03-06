# crpi.py
import smbus
import socket
import struct
import logging
import configparser

# ... (constants and configuration remain the same)

def handle_state_changes(address, data):
  # ... (existing code remains the same)
  
  # Send updated control states to DRPi
  data_to_send = struct.pack('BB' + CONTROL_STATE_FORMAT,
                             address,
                             new_gain,
                             int(new_phase),
                             int(new_phantom_power),
                             int(new_input_impedance),
                             int(new_pad),
                             int(new_high_pass_filter),
                             int(new_mute))
  try:
    device_rpi_socket.sendall(data_to_send)
  except Exception as e:
    logging.error(f"Error sending control states to DRPi: {str(e)}")

def handle_device_state_changes(address, data):
  # Update stored control states based on received data from DRPi
  control_states[address]['gain'] = data[0]
  control_states[address]['phase'] = data[1]
  control_states[address]['phantom_power'] = data[2]
  control_states[address]['input_impedance'] = data[3]
  control_states[address]['pad'] = data[4]
  control_states[address]['high_pass_filter'] = data[5]
  control_states[address]['mute'] = data[6]
  
  # Send updated control states to the corresponding CA
  send_control_states(address)

def main():
  # ... (existing code remains the same)
  
  while True:
    # ... (existing code remains the same)
    
    # Check for incoming data from DRPi
    try:
      data = device_rpi_socket.recv(I2C_BUFFER_SIZE)
      if len(data) == struct.calcsize('B' + CONTROL_STATE_FORMAT):
        unpacked_data = struct.unpack('B' + CONTROL_STATE_FORMAT, data)
        address = unpacked_data[0]
        handle_device_state_changes(address, unpacked_data[1:])
    except Exception as e:
      logging.error(f"Error receiving data from DRPi: {str(e)}")

# ... (remaining code remains the same)