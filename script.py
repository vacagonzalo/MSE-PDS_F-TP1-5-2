from serial import Serial
import numpy as np
import matplotlib.pyplot as plt




if __name__ == "__main__":
    SIZE = 1000
    index = 0
    values = np.arange(SIZE)
    val = 0
    while True:
        with Serial('/dev/ttyACM0', 115200, timeout=1) as usb:
            x = usb.read()
            if(x == b'<'):
                val = int.from_bytes(usb.read(2), byteorder='big')
                if(index < SIZE):
                    values[index] = val
                    index = index + 1
                else:
                    maximo = np.amax(values)
                    minimo = np.amin(values)
                    rms = np.sqrt(np.mean(values**2))
                    print('maximo', maximo, 'minimo', minimo, 'rms', rms)
                    index = 0
