import math

import numpy as np


class IRCommand:
    def __init__(self, name, type, frequency, duty_cycle, data):
        self.name = name
        self.type = type
        self.frequency = frequency
        self.duty_cycle = duty_cycle
        self.data = data

    @staticmethod
    def from_dict(dictionary):
        """
            Create an IRCommand from a dictionary
            :param dictionary: Structure that contains IR command's detail: name, type, frequency, duty_cycle, data
            :return:
        """
        return IRCommand(dictionary['name'], dictionary['type'], int(dictionary['frequency']), float(dictionary['duty_cycle']), dictionary['data'])

    def get_sampling_step(self):
        """
            Get the sampling step of the IR command
            :return: the sampling step of the IR command
        """
        return 1 / int(self.frequency)

    def get_duration_list(self):
        return [int(raw_data) * 10**-6 for raw_data in self.data.split(' ')]

    def get_duration(self):
        """
            Get the duration of the IR command
            :return: total duration of the IR command
        """
        duration = 0
        durations_s = self.get_duration_list()

        for interval in durations_s:
            duration += interval

        return duration

    def get_signal(self):
        """
            Compute the signal of the IR command, x and y
            :return: x_signal, y_signal the X/Y signal of the IR command
        """
        index = 0
        state = True
        current_duration = 0

        total_duration = self.get_duration()
        sampling_step = self.get_sampling_step()
        durations_s = self.get_duration_list()

        # 1/38000 ==> 38kHz, general IR frequency
        x_signal = np.linspace(0, total_duration, int(total_duration / (1 / 38000)))
        y_signal = []

        for k in range(math.floor(total_duration / sampling_step)):
            if k * sampling_step - current_duration > durations_s[index]:
                current_duration = k * sampling_step
                index += 1
                state = not state

            y_signal += [1 if state else 0]

        return x_signal, y_signal

    def get_registers(self, arr):
        """
            Get the CCR / RCR registers to build the IR command
            :param arr: The value of the ARR to compute the RCR values
            :return:
        """
        arr_list = []
        ccr_list = []  # Duty cycle of the on-going signal
        rcr_list = []  # Number of pulse per on-going signal

        x_signal, y_signal = self.get_signal()

        rcr = 0
        last_value = y_signal[0]
        for k in range(len(x_signal)):
            if y_signal[k] != last_value:
                arr_list += [arr]
                rcr_list += [rcr]

                if last_value >= 0.5:
                    ccr_list += [int(arr * self.duty_cycle)]
                else:
                    ccr_list += [0]

                rcr = 0

            last_value = y_signal[k]
            rcr += 1

        return arr_list, ccr_list, rcr_list