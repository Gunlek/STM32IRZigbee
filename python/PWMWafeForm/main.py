import matplotlib.pyplot as plt

def generate_pwm(period, duty_cycle, offset=0):
    duty_period = period * duty_cycle
    x = [offset, duty_period + offset, duty_period + offset, period + offset]
    y = [1, 1, 0, 0]

    return x, y

# Article TIM
# pwm_y = [1, 1, 0, 0, 1, 1, 0, 0]
# pwm_x = [0, 0.25, 0.25, 1, 1, 1.25, 1.25, 2]

# Article TIM + DMA
index = 0
offset = 0
period = 1
duty = 0.25

pwm_x = []
pwm_y = []
for k in range(15):
    if k % 5 == 0:
        duty = 0.75 if duty == 0.25 else 0.25

    x, y = generate_pwm(period, duty, k)
    pwm_x += x
    pwm_y += y

print(pwm_x)

plt.plot(pwm_x, pwm_y)
plt.show()