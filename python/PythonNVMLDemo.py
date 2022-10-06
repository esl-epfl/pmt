import time

import pmt

@pmt.measure('nvml')
def my_kernel():
   time.sleep(1)

if __name__ == "__main__":
   print(my_kernel())
