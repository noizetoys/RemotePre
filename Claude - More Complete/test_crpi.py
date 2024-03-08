# test_crpi.py
import unittest
from unittest.mock import MagicMock, patch
import struct
from crpi import handle_state_changes, handle_device_state_changes

class TestCRPi(unittest.TestCase):
    def setUp(self):
        self.control_states = {
            0x03: {
                'gain': 50,
                'phase': False,
                'phantom_power': False,
                'input_impedance': False,
                'pad': False,
                'high_pass_filter': False,
                'mute': False
            }
        }
        self.device_rpi_socket = MagicMock()

    @patch('crpi.send_control_states')
    def test_handle_state_changes(self, mock_send_control_states):
        address = 0x03
        data = [60, 1, 1, 1, 1, 1, 1]
        handle_state_changes(address, data)
        self.assertEqual(self.control_states[address]['gain'], 60)
        self.assertTrue(self.control_states[address]['phase'])
        self.assertTrue(self.control_states[address]['phantom_power'])
        self.assertTrue(self.control_states[address]['input_impedance'])
        self.assertTrue(self.control_states[address]['pad'])
        self.assertTrue(self.control_states[address]['high_pass_filter'])
        self.assertTrue(self.control_states[address]['mute'])
        self.device_rpi_socket.sendall.assert_called_once()
        mock_send_control_states.assert_called_once_with(address)

    @patch('crpi.send_control_states')
    def test_handle_device_state_changes(self, mock_send_control_states):
        address = 0x03
        data = [70, 0, 0, 0, 0, 0, 0]
        handle_device_state_changes(address, data)
        self.assertEqual(self.control_states[address]['gain'], 70)
        self.assertFalse(self.control_states[address]['phase'])
        self.assertFalse(self.control_states[address]['phantom_power'])
        self.assertFalse(self.control_states[address]['input_impedance'])
        self.assertFalse(self.control_states[address]['pad'])
        self.assertFalse(self.control_states[address]['high_pass_filter'])
        self.assertFalse(self.control_states[address]['mute'])
        mock_send_control_states.assert_called_once_with(address)

if __name__ == '__main__':
    unittest.main()