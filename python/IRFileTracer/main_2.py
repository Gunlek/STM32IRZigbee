import math

import matplotlib.pyplot as plt

from pwm_generator import pwm_generate_signal, local_pwm_converter, signal_to_pwm
import utils
import assets.path
import numpy as np

ir_dict, requested_cmd = utils.load_ir_file(assets.path.FILE_PATH)

sampling_frequency = int(ir_dict[requested_cmd]['frequency'])
sampling_duty_cycle = float(ir_dict[requested_cmd]['duty_cycle'])
sampling_step = 1 / sampling_frequency

print('Sampling frequency: ', sampling_frequency)
print('Sampling duty cycle: ', sampling_duty_cycle)

# --- Get the duration of the payload in microseconds
data_requested = ir_dict[requested_cmd]['data']
payload_duration_in_s = 0
durations_s = [int(raw_data) * 10**-6 for raw_data in data_requested.split(' ')]
for interval in durations_s:
    payload_duration_in_s += interval

print("Temps de trame: " + str(payload_duration_in_s))
print("Temps de trame (us): " + str(payload_duration_in_s * 10**6))


x_signal = np.linspace(0, payload_duration_in_s, int(payload_duration_in_s / (1/38000)))
y_signal = []

print(durations_s)
index = 0
state = True
current_duration = 0
for k in range(math.floor(payload_duration_in_s / sampling_step)):
    if k * sampling_step - current_duration > durations_s[index]:
        current_duration = k * sampling_step
        index += 1
        state = not state

    y_signal += [1 if state else 0]

ccr_list = []    # Duty cycle of the on-going signal
rcr_list = []    # Number of pulse per on-going signal

rcr = 0
last_value = y_signal[0]
for k in range(len(x_signal)):
    if y_signal[k] != last_value:
        rcr_list += [rcr]

        if last_value >= 0.5:
            ccr_list += [0.3333333333333333]
        else:
            ccr_list += [0]

        rcr = 0

    last_value = y_signal[k]
    rcr += 1

print('RCR List: ', rcr_list)
print('CCR List: ', ccr_list)

arr_list = [420 for k in range(len(ccr_list))]

def generate_code(arr_list, rcr_list, ccr_list):
    lines = []
    for k in range(len(arr_list)):
        lines += [(str(arr_list[k]), str(rcr_list[k]), str(int(arr_list[k] * ccr_list[k])))]

    code = 'uint32_t aSRC_Buffer[' + str(len(arr_list)) + '] = {\n'
    for line in lines:
        code += ', '.join(line) + ',\n'
    code += '}'

    return code


print(generate_code(arr_list, rcr_list, ccr_list))

# def generate_from_rcr_ccr(rcr, ccr, frequency):
#     pwm_generator.pwm(frequency, ccr, )

plt.plot(x_signal, y_signal)
plt.ylim(-5, 5)

plt.show()




