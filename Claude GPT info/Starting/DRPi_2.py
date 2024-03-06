# Device RPi (DRPi)


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
I2C_BUFFER_SIZE = config.getint('I2C', 'BUFFER_SIZE')

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

def receive_control_states(sock):
    try:
        data = sock.recv(I2C_BUFFER_SIZE)
        if len(data) == struct.calcsize('B' + CONTROL_STATE_FORMAT):
            unpacked_data = struct.unpack('B' + CONTROL_STATE_FORMAT, data)
            address = unpacked_data[0]
            control_state = {
                'gain': unpacked_data[1],
                'phase': unpacked_data[2],
                'phantom_power': unpacked_data[3],
                'input_impedance': unpacked_data[4],
                'pad': unpacked_data[5],
                'high_pass_filter': unpacked_data[6]
            }
            return address, control_state
    except Exception as e:
        logging.error(f"Error receiving control states: {str(e)}")
    return None, None

def send_control_states(address, control_state):
    try:
        packed_data = struct.pack(CONTROL_STATE_FORMAT,
                                  control_state['gain'],
                                  int(control_state['phase']),
                                  int(control_state['phantom_power']),
                                  int(control_state['input_impedance']),
                                  int(control_state['pad']),
                                  int(control_state['high_pass_filter']))
        i2c_bus.write_i2c_block_data(address, 0x00, list(packed_data))
        return True
    except Exception as e:
        logging.error(f"Error sending control states to address {address}: {str(e)}")
    return False

def handle_crpi_connection(sock):
    while True:
        address, control_state = receive_control_states(sock)
        if address is not None and control_state is not None:
            send_control_states(address, control_state)

def main():
    # Set up socket connection with CRPi
    crpi_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    crpi_socket.bind((CRPI_IP, CRPI_PORT))
    crpi_socket.listen(1)
    logging.info(f"DRPi listening on {CRPI_IP}:{CRPI_PORT}")

    while True:
        # Wait for a connection from CRPi
        sock, addr = crpi_socket.accept()
        logging.info(f"Connected to CRPi: {addr}")

        # Handle the CRPi connection
        handle_crpi_connection(sock)

if __name__ == '__main__':
    main()