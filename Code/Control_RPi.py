import smbus
import RPi.GPIO as GPIO
import time
import requests
from flask import Flask, request

app = Flask(__name__)

# Raspberry Pi configuration
bus = smbus.SMBus(1)  # For Raspberry Pi 2 and older 1 Model A/B boards
SLAVE_ADDRESS = 0x08  # Default slave address

# GPIO configuration for multiplexer control
mux_control_pins = [10, 11, 12, 13]  # Example GPIO pins connected to the multiplexer
GPIO.setmode(GPIO.BOARD)
for pin in mux_control_pins:
    GPIO.setup(pin, GPIO.OUT)

# Dictionary to store device data
device_data_table = {}

def select_slave(device_id):
    # Function to select the slave using multiplexer
    GPIO.output(mux_control_pins, GPIO.LOW)  # Deselect all slaves
    binary_id = bin(device_id)[2:].zfill(4)  # Convert device ID to binary
    for i, value in enumerate(binary_id):
        GPIO.output(mux_control_pins[i], int(value))

def read_data_from_slave(device_id):
    # Function to read data from the specified slave
    select_slave(device_id)
    for attempt in range(3):  # Retry mechanism for reliability
        try:
            # Request data from Arduino
            bus.write_byte(SLAVE_ADDRESS, 0)
            time.sleep(0.1)  # Short delay to allow the Arduino to respond
            # Read response from Arduino
            response = bus.read_i2c_block_data(SLAVE_ADDRESS, 0)
            return response
        except Exception as e:
            print(f"Error reading data from slave {device_id}: {e}")
            time.sleep(0.1)  # Short delay before retrying
    return None

def parse_data(response):
    # Function to parse the received data from the Arduino
    # Check if the response length is valid
    if len(response) != 11:  # Adjust the length based on your struct size
        print("Invalid response length")
        return None
    # Extract data from response
    device_id = response[0]
    bool_states = response[1:7]
    volume = (response[7] << 8) | response[8]
    mute = bool(response[9])
    return {
        'device_id': device_id,
        'bool_states': bool_states,
        'volume': volume,
        'mute': mute
    }

def calculate_crc(data):
    # Calculate CRC checksum for data
    polynomial = 0x31  # CRC-8-CCITT polynomial
    crc = 0xFF  # Initial value
    for byte in data:
        crc ^= byte
        for _ in range(8):
            if crc & 0x80:
                crc = (crc << 1) ^ polynomial
            else:
                crc <<= 1
    return crc

def validate_crc(data_with_crc):
    # Validate CRC checksum for received data
    data = data_with_crc[:-1]
    received_crc = data_with_crc[-1]
    calculated_crc = calculate_crc(data)
    return received_crc == calculated_crc

@app.route('/update_data', methods=['POST'])
def update_data():
    data = request.json
    device_id = data['device_id']
    data_with_crc = data['data']
    if validate_crc(data_with_crc):
        device_data_table[device_id] = data_with_crc[:-1]  # Remove CRC before storing
        print("Updated data for device ID", device_id)
        print("New Data:", device_data_table[device_id])
        return 'Data updated successfully'
    else:
        print("CRC validation failed for data received from device ID", device_id)
        return 'CRC validation failed'

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
