import time

from pmt_decorator import pmt

@pmt('nvml')
def my_kernel():
   time.sleep(10)

if __name__ == "__main__":
   print(my_kernel())
