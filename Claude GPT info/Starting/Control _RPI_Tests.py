# Control RPI Tests
# Mock DRPi server


import smbus
import socket
import time

# Define mock CA I2C address
MOCK_CA_ADDRESS = 0x20

# Define mock control state data
mock_control_states = {
    'gain': 0,
    'phase': False,
    'phantom_power': False,
    'input_impedance': False,
    'pad': False,
    'high_pass_filter': False
}

# Initialize I2C bus
i2c_bus = smbus.SMBus(1)

# Set up socket server
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(('localhost', 8000))
server_socket.listen(1)
print('Mock DRPi server listening on localhost:8000')

def send_mock_control_states(address):
    data = [
        mock_control_states['gain'],
        int(mock_control_states['phase']),
        int(mock_control_states['phantom_power']),
        int(mock_control_states['input_impedance']),
        int(mock_control_states['pad']),
        int(mock_control_states['high_pass_filter'])
    ]
    i2c_bus.write_i2c_block_data(address, 0x00, data)

def receive_mock_control_states(address):
    data = i2c_bus.read_i2c_block_data(address, 0x00, 6)
    mock_control_states['gain'] = data[0]
    mock_control_states['phase'] = bool(data[1])
    mock_control_states['phantom_power'] = bool(data[2])
    mock_control_states['input_impedance'] = bool(data[3])
    mock_control_states['pad'] = bool(data[4])
    mock_control_states['high_pass_filter'] = bool(data[5])
    print(f"Received: {mock_control_states}")

def handle_client(client_socket):
    while True:
        data = client_socket.recv(1024)
        if not data:
            break
        print(f"Received from CRPi: {data.decode()}")

while True:
    print("Sending mock control states to CRPi...")
    send_mock_control_states(MOCK_CA_ADDRESS)
    time.sleep(1)

    print