# test_drpi.py
import unittest
from unittest.mock import MagicMock, patch
import struct
from drpi import receive_control_states, send_control_states, send_control_states_to_crpi, handle_crpi_connection

class TestDRPi(unittest.TestCase):
    def setUp(self):
        self.sock = MagicMock()
        self.i2c_bus = MagicMock()
        self.crpi_socket = MagicMock()
        self.control_states = {
            0x08: {
                'gain': 50,
                'phase': False,
                'phantom_power': False,
                'input_impedance': False,
                'pad': False,
                'high_pass_filter': False,
                'mute': False
            }
        }

    def test_receive_control_states(self):
        data = struct.pack('BBBBBBB', 60, 1, 1, 1, 1, 1, 1)
        self.sock.recv.return_value = data
        control_state = receive_control_states(self.sock)
        self.assertEqual(control_state['gain'], 60)
        self.assertTrue(control_state['phase'])
        self.assertTrue(control_state['phantom_power'])
        self.assertTrue(control_state['input_impedance'])
        self.assertTrue(control_state['pad'])
        self.assertTrue(control_state['high_pass_filter'])
        self.assertTrue(control_state['mute'])

    def test_send_control_states(self):
        control_state = {
            'gain': 70,
            'phase': True,
            'phantom_power': True,
            'input_impedance': True,
            'pad': True,
            'high_pass_filter': True,
            'mute': True
        }
        result = send_control_states(control_state)
        self.assertTrue(result)
        packed_data = struct.pack('BBBBBBB', 70, 1, 1, 1, 1, 1, 1)
        data = [0xAA] + list(packed_data) + [0xBB]
        self.i2c_bus.write_i2c_block_data.assert_called_once_with(0x08, 0x00, data)

    def test_send_control_states_to_crpi(self):
        address = 0x08
        control_state = {
            'gain': 80,
            'phase': False,
            'phantom_power': False,
            'input_impedance': False,
            'pad': False,
            'high_pass_filter': False,
            'mute': False
        }
        send_control_states_to_crpi(address, control_state)
        data_to_send = struct.pack('BBBBBBBB', 0x08, 80, 0, 0, 0, 0, 0, 0)
        self.crpi_socket.sendall.assert_called_once_with(data_to_send)

    @patch('drpi.receive_control_states')
    @patch('drpi.send_control_states')
    @patch('drpi.send_control_states_to_crpi')
    def test_handle_crpi_connection(self, mock_send_control_states_to_crpi, mock_send_control_states, mock_receive_control_states):
        mock_receive_control_states.side_effect = [
            {'gain': 90, 'phase': True, 'phantom_power': True, 'input_impedance': True, 'pad': True, 'high_pass_filter': True, 'mute': True},
            None
        ]
        self.i2c_bus.read_i2c_block_data.return_value = [0xAA, 100, 0, 0, 0, 0, 0, 0, 0xBB]
        handle_crpi_connection(self.sock)
        mock_send_control_states.assert_called_once_with({'gain': 90, 'phase': True, 'phantom_power': True, 'input_impedance': True, 'pad': True, 'high_pass_filter': True, 'mute': True})
        mock_send_control_states_to_crpi.assert_called_once_with(0x08, {'gain': 100, 'phase': False, 'phantom_power': False, 'input_impedance': False, 'pad': False, 'high_pass_filter': False, 'mute': False})

if __name__ == '__main__':
    unittest.main()