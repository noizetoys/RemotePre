import smbus
import time

# Raspberry Pi configuration
bus = smbus.SMBus(1)  # For Raspberry Pi 2 and older 1 Model A/B boards
SLAVE_ADDRESS = 0x08  # Default slave address

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

def receive_data_from_control_pi():
    # Function to receive data from the Control Raspberry Pi
    while True:
        try:
            # Read data from I2C bus
            response = bus.read_i2c_block_data(SLAVE_ADDRESS, 0)
            # Check if CRC is valid
            if validate_crc(response):
                return response[:-1]  # Return data without CRC
            else:
                print("CRC validation failed for received data")
                # Handle CRC validation failure (e.g., resend request)
        except Exception as e:
            print(f"Error receiving data from Control Raspberry Pi: {e}")
            # Handle communication error (e.g., retry or abort)

# Main loop
while True:
    data = receive_data_from_control_pi()
    # Process received data
    print("Received data from Control Raspberry Pi:", data)
    # Your code to handle received data
    time.sleep(1)  # Example delay
