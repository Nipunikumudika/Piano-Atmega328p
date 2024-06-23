
from array import array
from time import sleep
import serial
import pygame
from pygame.mixer import Sound, get_init, pre_init
import time

class Note(Sound):

    def __init__(self, frequency,waveform, volume=.1):
        self.frequency = frequency
        self.waveform = waveform
        Sound.__init__(self, self.build_samples())
        self.set_volume(volume)

    def build_samples(self):
        if(self.waveform==1):
            period = int(round(get_init()[0] / self.frequency))
            samples = array("h", [0] * period)
            amplitude = 2 ** (abs(get_init()[1]) - 1) - 1
            for time in range(period):
                if time < period / 2:
                    samples[time] = amplitude
                else:
                    samples[time] = -amplitude
            return samples
        else:
            period = int(round(get_init()[0] / self.frequency))
            samples = array("h", [0] * period)
            amplitude = 2 ** (abs(get_init()[1]) - 1) - 1
            half_period = period // 2
            for time in range(period):
                if time < half_period:
                    sample = int(amplitude * (time / half_period))
                else:
                    sample = int(amplitude * ((period - time) / half_period))
                samples[time] = max(min(sample, amplitude), -amplitude)  # Clamp sample value
            return samples


if __name__ == "__main__":
    # Initialize Pygame
    pre_init(44100, -16, 1, 1024)
    pygame.init()
    serial_port = serial.Serial(port='COM9', baudrate=9600, timeout=0, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE)
    first_frequency=0
    first_instrument=-1
    note_instance=0
    buffer = bytearray()
    # Initialize the timer
    last_received_time = time.time()
    timeout = 0.2  # 200 milliseconds
    while True:
        data = serial_port.read(1024)
        #print(data)
        #print(data)
        current_time = time.time()
        if data:
            print(data)
            last_received_time = current_time
            buffer.extend(data)
            while b'\r\n' in buffer:
                line, buffer = buffer.split(b'\r\n', 1)
                try:
                    line_str = line.decode('utf-8').strip()
                    parts = line_str.split()
                    half_wave_length = float(parts[1])
                    sec_frequency = 1000 / (2 * half_wave_length)
                    sec_instrument=int(parts[0])
                    print(line_str)
                    if(first_frequency!=sec_frequency or first_instrument!=sec_instrument):
                        #print(first_frequency)
                        #print(sec_frequency)
                        if (note_instance):  # Check if note_instance is defined
                            #print("have note instance")
                            note_instance.stop()
                        note_instance = Note(sec_frequency, waveform=sec_instrument)
                        note_instance.play(-1)
                        first_frequency=sec_frequency
                        first_instrument=sec_instrument
                    # note_instance.play(-1)
                    
                except (ValueError, IndexError):
                    print("Invalid data received:", line_str)
        else:
            if note_instance and (current_time - last_received_time > timeout):
                note_instance.stop()
                first_frequency=0
                note_instance = None  # Reset note_instance to avoid repeated stopping
                print("Sound stopped due to timeout")