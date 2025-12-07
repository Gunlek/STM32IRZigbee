# 1- Read the file with the Parser
# 2- Prepare the data to be stored
from matplotlib import pyplot as plt

from IRParser import IRParser
from AT24C08 import *

def generate_pwm_signal(ARR_list, RCR_list, CCR_list, timer_clock_hz=38000):
    """
    Reconstructs and plots the PWM signal produced by an STM32 timer
    with ARR, RCR and CCR values updated by DMA.

    :param ARR_list: list of ARR values for each segment
    :param RCR_list: list of repetition counter values for each segment
    :param CCR_list: list of CCR values for each segment
    :param timer_clock_hz: timer clock in Hz (default = 1 MHz)
    """

    assert len(ARR_list) == len(RCR_list) == len(CCR_list), \
        "ARR, RCR and CCR lists must have the same length"

    time = []
    signal = []
    t = 0.0

    dt = 1.0 / timer_clock_hz  # sampling resolution

    for ARR, RCR, CCR in zip(ARR_list, RCR_list, CCR_list):

        # One timer period duration
        period_time = (ARR + 1) * dt

        # Repeat according to RCR+1
        for _ in range(RCR + 1):

            steps = ARR + 1  # one tick per timer count
            for counter in range(steps):
                time.append(t)
                signal.append(1 if counter < CCR else 0)
                t += dt

    # Plot the signal
    plt.figure(figsize=(12, 5))
    plt.plot(time, signal, drawstyle="steps-pre")
    plt.xlabel("Time (s)")
    plt.ylabel("PWM output")
    plt.title("Reconstructed PWM signal from ARR/RCR/CCR")
    plt.grid(True)
    plt.show()

if __name__ == '__main__':
    path = './assets/Dyson.ir' # input('Enter the path of IR commands file: ')
    parser = IRParser(path)

    print("Available commands:", parser.get_available_commands())
    command = input('Enter the command: ')
    commands = command.split(", ")

    mmy = AT24C08()

    stored_commands = {}

    offset = 0
    for cmd_name in commands:

        cmd = parser.get_command(cmd_name)
        arr, ccr, rcr = cmd.get_registers(arr=210)

        mmy.store(address=offset, value=len(rcr))

        assert(offset + len(rcr) < mmy.bytes_per_page * mmy.pages)

        for (r, address) in zip(rcr, range(len(rcr))):
            # --- Shift address +1 as the first byte is the total length
            mmy.store(address=offset + address+1, value=r)

        stored_commands[cmd_name] = offset
        offset += len(rcr) + 1

    print(mmy)
    print(stored_commands)

    print(mmy.memory[0][67::])
    mmy.commit()
