import time

from pmt import *

@pmt_meas('nvml')
def my_kernel():
   time.sleep(1)

if __name__ == "__main__":
   print(my_kernel())
