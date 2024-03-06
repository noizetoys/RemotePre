# CRPi.py


import smbus
import socket
import struct

# Define I2C constants
I2C_BUS = 1
I2C_ADDRESSES = []

# Define socket constants
DEVICE_RPI_IP = '192.168.1.100'
DEVICE_RPI_PORT = 8000

# I2C protocol constants
I2C_START_BYTE = 0xAA
I2C_END_BYTE = 0xBB

# Control state structure format
CONTROL_STATE_FORMAT = 'BBBBBBx'  # Padding byte added for alignment

# Initialize I2C bus and socket
i2c_bus = smbus.SMBus(I2C_BUS)
device_rpi_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
device_rpi_socket.connect((DEVICE_RPI_IP, DEVICE_RPI_PORT))

# Control state data structure
control_states = {}

def scan_i2c_devices():
    global I2C_ADDRESSES
    for address in range(0x03, 0x78):
        try:
            i2c_bus.write_byte(address, 0x00)
            I2C_ADDRESSES.append(address)
            control_states[address] = {
                'gain': 0,
                'phase': False,
                'phantom_power': False,
                'input_impedance': False,
                'pad': False,
                'high_pass_filter': False
            }
        except Exception:
            pass

def receive_control_states(address):
    try:
        data = i2c_bus.read_i2c_block_data(address, 0x00, 8)
        if data[0] == I2C_START_BYTE and data[-1] == I2C_END_BYTE:
            control_data = data[1:-1]
            unpacked_data = struct.unpack(CONTROL_STATE_FORMAT, bytes(control_data))
            control_states[address]['gain'] = unpacked_data[0]
            control_states[address]['phase'] = unpacked_data[1]
            control_states[address]['phantom_power'] = unpacked_data[2]
            control_states[address]['input_impedance'] = unpacked_data[3]
            control_states[address]['pad'] = unpacked_data[4]
            control_states[address]['high_pass_filter'] = unpacked_data[5]
            return True
    except Exception:
        pass
    return False

def send_control_states(address):
    try:
        packed_data = struct.pack(CONTROL_STATE_FORMAT,
                                  control_states[address]['gain'],
                                  int(control_states[address]['phase']),
                                  int(control_states[address]['phantom_power']),
                                  int(control_states[address]['input_impedance']),
                                  int(control_states[address]['pad']),
                                  int(control_states[address]['high_pass_filter']))
        data = [I2C_START_BYTE] + list(packed_data) + [I2C_END_BYTE]
        i2c_bus.write_i2c_block_data(address, 0x00, data)
        return True
    except Exception:
        pass
    return False

def handle_state_changes(address, data):
    new_gain = data[0]
    new_phase = data[1]
    new_phantom_power = data[2]
    new_input_impedance = data[3]
    new_pad = data[4]
    new_high_pass_filter = data[5]

    if (
        control_states[address]['gain'] != new_gain or
        control_states[address]['phase'] != new_phase or
        control_states[address]['phantom_power'] != new_phantom_power or
        control_states[address]['input_impedance'] != new_input_impedance or
        control_states[address]['pad'] != new_pad or
        control_states[address]['high_pass_filter'] != new_high_pass_filter
    ):
        control_states[address]['gain'] = new_gain
        control_states[address]['phase'] = new_phase
        control_states[address]['phantom_power'] = new_phantom_power
        control_states[address]['input_impedance'] = new_input_impedance
        control_states[address]['pad'] = new_pad
        control_states[address]['high_pass_filter'] = new_high_pass_filter

        # Send updated control states to DRPi
        data_to_send = struct.pack('BB' + CONTROL_STATE_FORMAT,
                                   address,
                                   new_gain,
                                   int(new_phase),
                                   int(new_phantom_power),
                                   int(new_input_impedance),
                                   int(new_pad),
                                   int(new_high_pass_filter))
        device_rpi_socket.sendall(data_to_send)

def main():
    # Scan for connected CAs
    scan_i2c_devices()

    # Set up event loop to handle incoming data
    while True:
        # Check for incoming data from CAs
        for address in I2C_ADDRESSES:
            if receive_control_states(address):
                handle_state_changes(address, control_states[address].values())

        # Check for incoming data from DRPi
        # ...

if __name__ == '__main__':
    main()