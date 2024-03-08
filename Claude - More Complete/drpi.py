# drpi.py
import socket
import struct
import smbus
import logging
import configparser

# Load configuration from file
config = configparser.ConfigParser()
config.read('config.ini')

# Define socket constants
CRPI_IP = config.get('Socket', 'CRPI_IP')
CRPI_PORT = config.getint('Socket', 'CRPI_PORT')

# Define I2C constants
I2C_BUS = config.getint('I2C', 'BUS')
I2C_DEVICE_ADDRESS = config.getint('I2C', 'DEVICE_ADDRESS')
I2C_BUFFER_SIZE = config.getint('I2C', 'BUFFER_SIZE')
I2C_START_BYTE = config.getint('Protocol', 'START_BYTE')
I2C_END_BYTE = config.getint('Protocol', 'END_BYTE')

# Control state structure format
CONTROL_STATE_FORMAT = config.get('Protocol', 'CONTROL_STATE_FORMAT')

# Logging configuration
logging.basicConfig(level=logging.DEBUG,
                    format='%(asctime)s - %(levelname)s - %(message)s',
                    handlers=[
                        logging.FileHandler('drpi.log'),
                        logging.StreamHandler()
                    ])

# Initialize I2C bus
i2c_bus = smbus.SMBus(I2C_BUS)

# Initialize socket connection with CRPi
crpi_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
crpi_socket.bind((CRPI_IP, CRPI_PORT))
crpi_socket.listen(1)
logging.info(f"DRPi listening on {CRPI_IP}:{CRPI_PORT}")

# Control state data structure
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
        data = [I2C_START_BYTE] + list(packed_data) + [I2C_END_BYTE]
        i2c_bus.write_i2c_block_data(I2C_DEVICE_ADDRESS, 0x00, data)
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
                data = i2c_bus.read_i2c_block_data(address, 0x00, I2C_BUFFER_SIZE)
                if data[0] == I2C_START_BYTE and data[-1] == I2C_END_BYTE:
                    control_data = data[1:-1]
                    unpacked_data = struct.unpack(CONTROL_STATE_FORMAT, bytes(control_data))
                    control_states[address]['gain'] = unpacked_data[0]
                    control_states[address]['phase'] = unpacked_data[1]
                    control_states[address]['phantom_power'] = unpacked_data[2]
                    control_states[address]['input_impedance'] = unpacked_data[3]
                    control_states[address]['pad'] = unpacked_data[4]
                    control_states[address]['high_pass_filter'] = unpacked_data[5]
                    control_states[address]['mute'] = unpacked_data[6]
                    send_control_states_to_crpi(address, control_states[address])
            except Exception as e:
                logging.error(f"Error receiving control states from address {address}: {str(e)}")

def main():
    while True:
        # Wait for a connection from CRPi
        sock, addr = crpi_socket.accept()
        logging.info(f"Connected to CRPi: {addr}")

        # Handle the CRPi connection
        handle_crpi_connection(sock)

if __name__ == '__main__':
    main()