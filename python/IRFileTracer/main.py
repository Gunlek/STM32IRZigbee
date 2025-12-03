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
payload_duration_in_us = 0
raw_data = [int(raw_data) for raw_data in data_requested.split(' ')]
for interval in raw_data:
    payload_duration_in_us += interval

payload_duration_in_s = payload_duration_in_us * 10**-6

x_signal = np.linspace(0, payload_duration_in_s, int(payload_duration_in_s / (1/38000)))
y_signal = []

# --- Generate a 0/1 signal to be plotted
current_state = True
lin_duration = np.linspace(0, payload_duration_in_us / 1000000, payload_duration_in_us)
for duration in raw_data:
    int_duration = int(duration)
    for k in range(int_duration):
        y_signal += [1 if current_state else 0]
    current_state = not current_state

print('Payload length: ' + str(payload_duration_in_us))

frequency = 1226    # Frequency of the PWM signal

# --- Split the signals per frequency (to see the duty-cycles)
last_split = 0
splitted_signal = []
nb_points_per_period = int(1 / frequency * 1000000)
for k in range(int(payload_duration_in_us / (1 / frequency * 1000000))):
    x_split = lin_duration[last_split:last_split + nb_points_per_period]
    y_split = y_signal[last_split:last_split + nb_points_per_period]
    splitted_signal.append([x_split, y_split])
    last_split += nb_points_per_period

    plt.plot(x_split, y_split)

# --- Convert the signal to a list of duty-cycles at a specific frequency
duty_cycles = signal_to_pwm((lin_duration, y_signal), frequency)

# Generated a signal from PWM duty-cycles list
# generated_signal_x, generated_signal_y = pwm_generate_signal(frequency, duty_cycles)
# plt.plot(generated_signal_x, generated_signal_y)

print('Corresponding Duty-Cycles at ' + str(frequency) + 'Hz: ')
print(duty_cycles)

# --- Compute the needed value to reproduce the PWM in STM32
ARR = 26100
CCR = [round(x * ARR) for x in duty_cycles]

print('Corresponding CCR values:')
print(CCR)
print('Length:')
print(len(CCR))

# --- Display raw BIN value
bin = ''.join(['1' if d > 0.1 else '0' for d in duty_cycles])
print('Binary value for this payload:')
print(bin)

generation_frequency = 38000
samples = math.ceil((payload_duration_in_us * 10**-6) * generation_frequency)


# duty_cycles = [0.33 for k in range(samples)]
# pwm_x, pwm_y = pwm_generate_signal(generation_frequency, duty_cycles)

step = 1 / generation_frequency

# durations = [int(x) for x in data_requested.split(' ')]
# rcr = [int((d * 10**-6) / (1/generation_frequency)) for d in durations]
# d_cycles = [((k + 1) % 2)*0.33 for k in range(len(durations))]
# ccr = [int(ARR * d) for d in d_cycles]

print(x_signal)
#for k in range(int((payload_duration_in_us * 10**-6) / step)):
#    print(np.interp(k * step, x_signal, y_signal), k*step, payload_duration_in_us* 10**-6)

# duty_cycles = []
# for k in range(len(rcr)):
#     duty_cycles += [d_cycles[k] for i in range(rcr[k])]

print(duty_cycles)
pwm_x, pwm_y = pwm_generate_signal(generation_frequency, duty_cycles)
# def compute_rcr(ref, target_frequency):
#     rcr_list = []
#     last_value = 1
#     rcr = 0
#     for k in range(len(ref)):
#         if ref[k] != last_value:
#             rcr_list += [rcr]
#             rcr = 0
#         else:
#             rcr += 1
#
#         last_value = ref[k]
#
#     print(rcr_list)
#
# rcr_list = compute_rcr(y_signal, generation_frequency)

offset = 1
pwm_y = [y - offset for y in pwm_y]
plt.plot(pwm_x, pwm_y)

plt.ylim(-5, 5)
plt.show()


